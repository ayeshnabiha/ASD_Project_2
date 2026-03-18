const express = require('express');
const { exec } = require('child_process');
const path = require('path');

const app = express();
const PORT = 3000;

app.use(express.json());
app.use(express.static(path.join(__dirname, '../frontend')));

const CPP_DIR = path.join(__dirname, '..');
const CPP_EXE = path.join(CPP_DIR, 'bin/main');

function runCppCommand(args) {
    return new Promise((resolve, reject) => {
        exec(`"${CPP_EXE}" ${args}`, { cwd: CPP_DIR }, (error, stdout, stderr) => {
            if (error) {
                reject({ error: error.message, stderr });
            } else {
                resolve(stdout.trim());
            }
        });
    });
}

// POST /enqueue - Tambah reservasi ke queue
app.post('/enqueue', async (req, res) => {
    try {
        // Support multiple input formats
        let nama, niu, tujuan, tanggal, durasi;
        
        if (req.body.nama) {
            // Format 1: { nama, niu, tujuan, tanggal, durasi }
            nama = req.body.nama;
            niu = req.body.niu;
            tujuan = req.body.tujuan;
            tanggal = req.body.tanggal;
            durasi = req.body.durasi;
        } else if (req.body.group_name) {
            // Format 2: { group_name, niu, purpose, date_day, date_month, date_year, duration, ... }
            nama = req.body.group_name;
            niu = req.body.niu;
            tujuan = req.body.purpose;
            durasi = req.body.duration;
            tanggal = `${req.body.date_year}-${req.body.date_month}-${req.body.date_day}`;
        } else {
            return res.status(400).json({ 
                status: 'error', 
                message: 'Missing required fields' 
            });
        }
        
        if (!nama || !niu || !tujuan || !tanggal || !durasi) {
            return res.status(400).json({ 
                status: 'error', 
                message: 'Missing required fields' 
            });
        }

        const args = `enqueue "${nama}" "${niu}" "${tujuan}" "${tanggal}" ${durasi}`;
        const result = await runCppCommand(args);
        
        try {
            const json = JSON.parse(result);
            res.json(json);
        } catch (e) {
            res.json({ status: 'success', message: result });
        }
    } catch (error) {
        res.status(500).json({ 
            status: 'error', 
            message: error.error || 'Failed to execute C++ command' 
        });
    }
});

// POST /dequeue - Proses reservasi (pindahkan dari queue ke stack)
app.post('/dequeue', async (req, res) => {
    try {
        const result = await runCppCommand('dequeue');
        
        try {
            const json = JSON.parse(result);
            res.json(json);
        } catch (e) {
            res.json({ status: 'success', message: result });
        }
    } catch (error) {
        res.status(500).json({ 
            status: 'error', 
            message: error.error || 'Failed to execute C++ command' 
        });
    }
});

// GET /queue - Tampilkan semua reservasi dalam queue (FIFO)
app.get('/queue', async (req, res) => {
    try {
        const result = await runCppCommand('showQueue');
        
        try {
            const json = JSON.parse(result);
            res.json(json);
        } catch (e) {
            res.json({ type: 'queue', data: [], size: 0 });
        }
    } catch (error) {
        res.status(500).json({ 
            status: 'error', 
            message: error.error || 'Failed to execute C++ command' 
        });
    }
});

// GET /history - Tampilkan histori reservasi (LIFO/Stack)
app.get('/history', async (req, res) => {
    try {
        const niuFilter = req.query.value;
        
        if (niuFilter) {
            // Get all reservations for specific NIU from queue and history
            const queueResult = await runCppCommand('showQueue');
            const historyResult = await runCppCommand('showStack');
            
            let allData = [];
            
            try {
                const queueJson = JSON.parse(queueResult);
                if (queueJson.data) {
                    allData = allData.concat(queueJson.data.filter(r => r.niu === niuFilter));
                }
            } catch (e) {}
            
            try {
                const historyJson = JSON.parse(historyResult);
                if (historyJson.data) {
                    allData = allData.concat(historyJson.data.filter(r => r.niu === niuFilter));
                }
            } catch (e) {}
            
            res.json({ type: 'history', data: allData, size: allData.length });
        } else {
            const result = await runCppCommand('showStack');
            
            try {
                const json = JSON.parse(result);
                res.json(json);
            } catch (e) {
                res.json({ type: 'stack', data: [], size: 0 });
            }
        }
    } catch (error) {
        res.status(500).json({ 
            status: 'error', 
            message: error.error || 'Failed to execute C++ command' 
        });
    }
});

// Root route - serve frontend
app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, '../frontend/index.html'));
});

app.listen(PORT, () => {
    console.log(`Server running at http://localhost:${PORT}`);
    console.log('C++ Program Path:', CPP_EXE);
});