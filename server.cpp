#include "httplib.h"
#include "database.h"
#include "linkedList.h"
#include "queue.h"
#include "stack.h"
#include "validation.h"

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>

// ─── JSON helpers (no external lib needed) ──────────────────────────────────

static std::string escapeJson(const std::string &s) {
    std::string out;
    for (char c : s) {
        if      (c == '"')  out += "\\\"";
        else if (c == '\\') out += "\\\\";
        else if (c == '\n') out += "\\n";
        else if (c == '\r') out += "\\r";
        else if (c == '\t') out += "\\t";
        else                out += c;
    }
    return out;
}

static std::string pad2(int n) {
    std::ostringstream ss;
    ss << std::setfill('0') << std::setw(2) << n;
    return ss.str();
}

static std::string purposeStr(int p) {
    if (p == 1) return "Praktikum";
    if (p == 2) return "Pelatihan";
    return "Other";
}

// Convert a linked-list chain (from loadReservations / getReservationsByNIU)
// into a JSON array string, then free the chain.
static std::string nodeChainToJson(Node *head) {
    std::string json = "[";
    Node *cur = head;
    bool first = true;
    while (cur) {
        if (!first) json += ",";
        first = false;
        const Reservation &r = cur->data;
        json += "{";
        json += "\"niu\":\""           + escapeJson(r.niu)        + "\",";
        json += "\"group_name\":\""    + escapeJson(r.group_name) + "\",";
        json += "\"purpose\":"         + std::to_string(r.purpose) + ",";
        json += "\"purpose_label\":\"" + purposeStr(r.purpose)    + "\",";
        json += "\"date\":\""
                + std::to_string(r.date_year) + "-"
                + pad2(r.date_month) + "-"
                + pad2(r.date_day)   + "\",";
        json += "\"date_display\":\""
                + pad2(r.date_day)   + "-"
                + pad2(r.date_month) + "-"
                + std::to_string(r.date_year) + "\",";
        json += "\"time_start\":\""
                + pad2(r.time_start_hour) + ":"
                + pad2(r.time_start_minutes) + "\",";
        json += "\"time_stop\":\""
                + pad2(r.time_stop_hour) + ":"
                + pad2(r.time_stop_minutes) + "\",";
        json += "\"duration\":"  + std::to_string(r.duration) + ",";
        json += "\"status\":\""  + escapeJson(r.status) + "\"";
        json += "}";
        cur = cur->next;
    }
    json += "]";

    // Free the chain
    Node *tmp;
    while (head) { tmp = head->next; delete head; head = tmp; }

    return json;
}

// Parse a JSON string value for a given key from a flat JSON body.
// Very simple – only handles the fields we produce ourselves.
static std::string jsonStr(const std::string &body, const std::string &key) {
    std::string search = "\"" + key + "\":\"";
    auto pos = body.find(search);
    if (pos == std::string::npos) return "";
    pos += search.size();
    auto end = body.find('"', pos);
    if (end == std::string::npos) return "";
    return body.substr(pos, end - pos);
}

static int jsonInt(const std::string &body, const std::string &key) {
    std::string search = "\"" + key + "\":";
    auto pos = body.find(search);
    if (pos == std::string::npos) return 0;
    pos += search.size();
    // skip optional quote (shouldn't be there for ints, but be safe)
    while (pos < body.size() && (body[pos] == '"' || body[pos] == ' ')) pos++;
    int val = 0;
    bool neg = false;
    if (pos < body.size() && body[pos] == '-') { neg = true; pos++; }
    while (pos < body.size() && isdigit(body[pos])) {
        val = val * 10 + (body[pos++] - '0');
    }
    return neg ? -val : val;
}

// ─── CORS helper ─────────────────────────────────────────────────────────────

static void addCors(httplib::Response &res) {
    res.set_header("Access-Control-Allow-Origin",  "*");
    res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    res.set_header("Access-Control-Allow-Headers", "Content-Type");
}

static bool readFileText(const std::string &path, std::string &content) {
    std::ifstream in(path, std::ios::in | std::ios::binary);
    if (!in) return false;
    std::ostringstream ss;
    ss << in.rdbuf();
    content = ss.str();
    return true;
}

// ─── main ────────────────────────────────────────────────────────────────────

int main() {
    sqlite3 *db;
    if (!initDatabase(&db, "reservations.db")) {
        std::cerr << "Failed to open database." << std::endl;
        return 1;
    }

    httplib::Server svr;

    // Serve dashboard static files so the UI is reachable at http://localhost:8080
    // (expects dashboard/index.html relative to the process working directory)
    if (!svr.set_mount_point("/", "./dashboard")) {
        std::cerr << "Warning: could not mount ./dashboard for static file serving." << std::endl;
    }

    // Pre-flight OPTIONS for all routes
    svr.Options(".*", [](const httplib::Request &, httplib::Response &res) {
        addCors(res);
        res.status = 204;
    });

    // Serve dashboard homepage directly at root URL
    svr.Get("/", [](const httplib::Request &, httplib::Response &res) {
        std::string html;
        if (!readFileText("dashboard/index.html", html)) {
            res.status = 500;
            res.set_content("Failed to load dashboard/index.html", "text/plain");
            return;
        }
        res.set_content(html, "text/html; charset=UTF-8");
    });

    // ── GET /api/reservations ── all non-cancelled/completed (the queue)
    svr.Get("/api/reservations", [&db](const httplib::Request &req, httplib::Response &res) {
        addCors(res);
        Node *head = loadReservations(db);  // already filters & marks completed
        res.set_content(nodeChainToJson(head), "application/json");
    });

    // ── GET /api/reservations/all ── every row (for history with status)
    svr.Get("/api/reservations/all", [&db](const httplib::Request &req, httplib::Response &res) {
        addCors(res);
        // We reuse getReservationsByNIU with a wildcard-like approach:
        // pass "%" to match everything via the LIKE query in database.cpp
        // But getReservationsByNIU binds the niu literally, so we just load all
        // via a small helper: load the active ones + we need all.
        // Simplest: query all via NIU = "%" pattern – the SQL uses LIKE ?.
        Node *head = getReservationsByNIU(db, "%");
        res.set_content(nodeChainToJson(head), "application/json");
    });

    // ── GET /api/reservations/history?niu=XXXXXX ──
    svr.Get("/api/reservations/history", [&db](const httplib::Request &req, httplib::Response &res) {
        addCors(res);
        std::string niu = req.get_param_value("niu");
        if (niu.empty()) {
            res.status = 400;
            res.set_content("{\"error\":\"niu required\"}", "application/json");
            return;
        }
        Node *head = getReservationsByNIU(db, niu);
        res.set_content(nodeChainToJson(head), "application/json");
    });

    // ── POST /api/reservations ── add a new reservation
    svr.Post("/api/reservations", [&db](const httplib::Request &req, httplib::Response &res) {
        addCors(res);

        const std::string &body = req.body;

        Reservation r;
        r.niu        = jsonStr(body, "niu");
        r.group_name = jsonStr(body, "group_name");
        r.purpose    = jsonInt(body, "purpose");
        r.duration   = jsonInt(body, "duration");

        // date: expect "YYYY-MM-DD"
        std::string date = jsonStr(body, "date");
        if (date.size() >= 10) {
            r.date_year  = std::stoi(date.substr(0, 4));
            r.date_month = std::stoi(date.substr(5, 2));
            r.date_day   = std::stoi(date.substr(8, 2));
        }

        // time_start: expect "HH:MM"
        std::string tstart = jsonStr(body, "time_start");
        if (tstart.size() >= 5) {
            r.time_start_hour    = std::stoi(tstart.substr(0, 2));
            r.time_start_minutes = std::stoi(tstart.substr(3, 2));
        }

        // Validate inputs
        if (r.niu.empty() || r.group_name.empty() ||
            !isValidDate(r.date_day, r.date_month, r.date_year) ||
            !isValidTime(r.time_start_hour, r.time_start_minutes) ||
            !isValidDuration(r.duration)) {
            res.status = 400;
            res.set_content("{\"error\":\"Invalid input\"}", "application/json");
            return;
        }

        // Match terminal app validation: reject passed date and passed start time for today
        if (isPassedDate(r.date_day, r.date_month, r.date_year) ||
            isPassedStartTime(r.date_day, r.date_month, r.date_year,
                              r.time_start_hour, r.time_start_minutes)) {
            res.status = 400;
            res.set_content("{\"error\":\"Date or start time has already passed\"}", "application/json");
            return;
        }

        // Compute stop time
        int totalMin = r.time_start_hour * 60 + r.time_start_minutes + r.duration;
        r.time_stop_hour    = (totalMin / 60) % 24;
        r.time_stop_minutes = totalMin % 60;
        r.status = "Accepted";

        // Conflict check
        if (hasTimeConflict(db, r)) {
            res.status = 409;
            res.set_content("{\"error\":\"Time slot conflict\"}", "application/json");
            return;
        }

        Node *node = createNode(r);
        bool saved = saveReservation(db, node);
        delete node;

        if (!saved) {
            res.status = 500;
            res.set_content("{\"error\":\"Failed to save\"}", "application/json");
            return;
        }

        updateStatus(db, r.niu, "Accepted");

        // Return the saved reservation as JSON
        std::string json = "{";
        json += "\"niu\":\""         + escapeJson(r.niu)        + "\",";
        json += "\"group_name\":\"" + escapeJson(r.group_name) + "\",";
        json += "\"purpose\":"      + std::to_string(r.purpose) + ",";
        json += "\"purpose_label\":\"" + purposeStr(r.purpose)  + "\",";
        json += "\"date\":\""
                + std::to_string(r.date_year) + "-"
                + pad2(r.date_month) + "-" + pad2(r.date_day) + "\",";
        json += "\"date_display\":\""
                + pad2(r.date_day) + "-" + pad2(r.date_month) + "-"
                + std::to_string(r.date_year) + "\",";
        json += "\"time_start\":\""
                + pad2(r.time_start_hour) + ":" + pad2(r.time_start_minutes) + "\",";
        json += "\"time_stop\":\""
                + pad2(r.time_stop_hour)  + ":" + pad2(r.time_stop_minutes)  + "\",";
        json += "\"duration\":"  + std::to_string(r.duration) + ",";
        json += "\"status\":\"Accepted\"";
        json += "}";

        res.status = 201;
        res.set_content(json, "application/json");
    });

    // ── GET /api/conflict ── check-only, no write
    svr.Get("/api/conflict", [&db](const httplib::Request &req, httplib::Response &res) {
        addCors(res);
        Reservation r;
        std::string date = req.get_param_value("date");
        std::string ts   = req.get_param_value("time_start");
        std::string dur  = req.get_param_value("duration");

        if (date.size() >= 10) {
            r.date_year  = std::stoi(date.substr(0, 4));
            r.date_month = std::stoi(date.substr(5, 2));
            r.date_day   = std::stoi(date.substr(8, 2));
        }
        if (ts.size() >= 5) {
            r.time_start_hour    = std::stoi(ts.substr(0, 2));
            r.time_start_minutes = std::stoi(ts.substr(3, 2));
        }
        r.duration = dur.empty() ? 0 : std::stoi(dur);

        bool conflict = hasTimeConflict(db, r);
        res.set_content(conflict ? "{\"conflict\":true}" : "{\"conflict\":false}",
                        "application/json");
    });

    std::cout << "Lab Reservation Server running at http://localhost:8080" << std::endl;
    std::cout << "Press Ctrl+C to stop." << std::endl;
    svr.listen("0.0.0.0", 8080);

    closeDatabase(db);
    return 0;
}
