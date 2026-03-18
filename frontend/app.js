/**
 * app.js - Frontend JavaScript for Lab Reservation System
 * 
 * This file handles:
 * 1. Pseudo login (NIU only, stored in localStorage)
 * 2. Add reservation form submission
 * 3. Queue view (FIFO representation)
 * 4. History view (LIFO representation)
 * 
 * API calls to Node.js backend using fetch
 * All Queue/Stack logic handled in C++
 */

// ============================================================
// GLOBAL STATE
// ============================================================

let currentNiu = localStorage.getItem('niu') || null;

// ============================================================
// INITIALIZATION
// ============================================================

document.addEventListener('DOMContentLoaded', function() {
    if (currentNiu) {
        showLoggedInState();
    }
    
    const today = new Date().toISOString().split('T')[0];
    document.getElementById('date').value = today;
});

// ============================================================
// NAVIGATION
// ============================================================

function showPage(page) {
    document.querySelectorAll('.page').forEach(p => p.classList.add('hidden'));
    document.getElementById('page-' + page).classList.remove('hidden');
    
    if (page === 'queue') {
        loadQueue();
    } else if (page === 'history') {
        loadHistory();
    }
}

// ============================================================
// PSEUDO LOGIN
// ============================================================

function login() {
    const niuInput = document.getElementById('login-niu');
    const niu = niuInput.value.trim();
    
    if (!niu || niu.length !== 6 || !/^\d+$/.test(niu)) {
        alert('Please enter a valid 6-digit NIU');
        return;
    }
    
    currentNiu = niu;
    localStorage.setItem('niu', niu);
    
    showLoggedInState();
    document.getElementById('niu').value = niu;
}

function logout() {
    currentNiu = null;
    localStorage.removeItem('niu');
    
    document.getElementById('login-section').classList.remove('hidden');
    document.getElementById('user-info').classList.add('hidden');
    document.getElementById('login-niu').value = '';
    document.getElementById('niu').value = '';
}

function showLoggedInState() {
    document.getElementById('login-section').classList.add('hidden');
    document.getElementById('user-info').classList.remove('hidden');
    document.getElementById('current-niu').textContent = currentNiu;
    document.getElementById('niu').value = currentNiu;
}

// ============================================================
// ADD RESERVATION
// ============================================================

async function submitReservation(event) {
    event.preventDefault();
    
    const messageDiv = document.getElementById('form-message');
    messageDiv.innerHTML = '';
    messageDiv.className = '';
    
    if (!currentNiu) {
        messageDiv.textContent = 'Please login first';
        messageDiv.className = 'error';
        return;
    }
    
    const groupName = document.getElementById('group-name').value;
    const niu = document.getElementById('niu').value;
    const dateInput = document.getElementById('date').value;
    const startTime = document.getElementById('start-time').value;
    const endTime = document.getElementById('end-time').value;
    const purpose = document.getElementById('purpose').value;
    
    const [year, month, day] = dateInput.split('-').map(Number);
    const [startHour, startMin] = startTime.split(':').map(Number);
    const [stopHour, stopMin] = endTime.split(':').map(Number);
    
    if (startHour > stopHour || (startHour === stopHour && startMin >= stopMin)) {
        messageDiv.textContent = 'Error: End time must be after start time';
        messageDiv.className = 'error';
        return;
    }
    
    const reservationData = {
        niu: niu,
        group_name: groupName,
        date_day: day,
        date_month: month,
        date_year: year,
        purpose: purpose,
        duration: ((stopHour * 60 + stopMin) - (startHour * 60 + startMin)),
        time_start_hour: startHour,
        time_start_minutes: startMin,
        time_stop_hour: stopHour,
        time_stop_minutes: stopMin,
        status: 'pending'
    };
    
    try {
        const response = await fetch('/enqueue', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(reservationData)
        });
        
        const result = await response.json();
        
        if (result.status === 'success') {
            messageDiv.textContent = '✓ Reservation created successfully!';
            messageDiv.className = 'success';
            
            document.getElementById('group-name').value = '';
            document.getElementById('start-time').value = '';
            document.getElementById('end-time').value = '';
            document.getElementById('purpose').value = '';
        } else {
            messageDiv.textContent = '✗ Error: ' + (result.message || 'Unknown error');
            messageDiv.className = 'error';
        }
    } catch (error) {
        messageDiv.textContent = '✗ Network error: ' + error.message;
        messageDiv.className = 'error';
    }
}

// ============================================================
// QUEUE VIEW (FIFO - First In First Out)
// ============================================================

async function loadQueue() {
    const tbody = document.getElementById('queue-body');
    const messageDiv = document.getElementById('queue-message');
    
    tbody.innerHTML = '';
    messageDiv.innerHTML = '';
    messageDiv.className = '';
    
    try {
        const response = await fetch('/queue');
        const result = await response.json();
        
        if (result.status === 'error') {
            messageDiv.textContent = 'Error loading queue: ' + result.message;
            messageDiv.className = 'error';
            return;
        }
        
        const reservations = result.data || [];
        
        if (reservations.length === 0) {
            messageDiv.textContent = 'No reservations in queue';
            messageDiv.className = '';
            return;
        }
        
        reservations.forEach((res, index) => {
            const row = document.createElement('tr');
            
            if (index === 0) {
                row.classList.add('queue-first');
                row.title = 'Front of Queue (First In)';
            }
            
            row.innerHTML = `
                <td>${index + 1}</td>
                <td>${res.group_name || res.nama || 'N/A'}</td>
                <td>${res.niu || 'N/A'}</td>
                <td>${res.date_day}/${res.date_month}/${res.date_year}</td>
                <td>${formatTime(res.time_start_hour, res.time_start_minutes)} - 
                    ${formatTime(res.time_stop_hour, res.time_stop_minutes)}</td>
                <td>${res.purpose || res.tujuan || 'N/A'}</td>
                <td>${res.status || 'pending'}</td>
            `;
            
            tbody.appendChild(row);
        });
        
        messageDiv.textContent = `Total: ${reservations.length} reservation(s) in queue`;
        
    } catch (error) {
        messageDiv.textContent = 'Network error: ' + error.message;
        messageDiv.className = 'error';
    }
}

// ============================================================
// HISTORY VIEW (LIFO - Last In First Out)
// ============================================================

async function loadHistory() {
    const tbody = document.getElementById('history-body');
    const messageDiv = document.getElementById('history-message');
    
    tbody.innerHTML = '';
    messageDiv.innerHTML = '';
    messageDiv.className = '';
    
    if (!currentNiu) {
        messageDiv.textContent = 'Please login to view your history';
        messageDiv.className = 'error';
        return;
    }
    
    try {
        const response = await fetch('/history?filter=niu&value=' + currentNiu);
        const result = await response.json();
        
        if (result.status === 'error') {
            messageDiv.textContent = 'Error loading history: ' + result.message;
            messageDiv.className = 'error';
            return;
        }
        
        let history = result.data || [];
        
        if (history.length === 0) {
            messageDiv.textContent = 'No reservation history';
            messageDiv.className = '';
            return;
        }
        
        history.forEach((res, index) => {
            const row = document.createElement('tr');
            
            row.innerHTML = `
                <td>${index + 1}</td>
                <td>${res.group_name || res.nama || 'N/A'}</td>
                <td>${res.date_day}/${res.date_month}/${res.date_year}</td>
                <td>${formatTime(res.time_start_hour, res.time_start_minutes)} - 
                    ${formatTime(res.time_stop_hour, res.time_stop_minutes)}</td>
                <td>${res.purpose || res.tujuan || 'N/A'}</td>
                <td>${res.status || 'done'}</td>
                <td>${index === 0 ? '← Most Recent (Top of Stack)' : ''}</td>
            `;
            
            tbody.appendChild(row);
        });
        
        messageDiv.textContent = `Total: ${history.length} reservation(s) in history`;
        
    } catch (error) {
        messageDiv.textContent = 'Network error: ' + error.message;
        messageDiv.className = 'error';
    }
}

// ============================================================
// UTILITY FUNCTIONS
// ============================================================

function formatTime(hour, minutes) {
    return String(hour || 0).padStart(2, '0') + ':' + String(minutes || 0).padStart(2, '0');
}