#ifndef NUBA_HTML_H
#define NUBA_HTML_H

#include <Arduino.h>

const char NUBA_PORTAL_HTML[] PROGMEM = R"=====(<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width,initial-scale=1">
    <title>nuba Config Page</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }

        body {
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Arial, sans-serif;
            background: #ffffff;
            min-height: 100vh;
            padding: 20px;
        }

        .container {
            max-width: 500px;
            margin: 40px auto;
            border: 1px solid #E0E0E0;
        }

        .header {
            background: #05B084;
            color: #fff;
            padding: 20px;
            border-bottom: 1px solid #05B084;
        }
        
        .header h1 {
            font-size: 20px;
            font-weight: 600;
            margin-bottom: 4px;
        }
        
        .header p {
            font-size: 11px;
        }
        
        .content {
            padding: 20px;
            background: #ffffff;
        }
        
        .form-group {
            margin-bottom: 16px;
        }
        
        .label {
            display: block;
            font-size: 13px;
            font-weight: 600;
            color: #161616;
            margin-bottom: 6px;
        }
        
        .input {
            width: 100%;
            padding: 10px;
            border: 1px solid #D0D0D0;
            border-radius: 0;
            font-size: 14px;
            background: #ffffff;
            color: #161616;
        }
        
        .input:focus {
            outline: none;
            border-color: #05B084;
        }
        
        .btn {
            width: 100%;
            padding: 12px;
            background: #05B084;
            color: #fff;
            border: 1px solid #05B084;
            border-radius: 0;
            font-size: 14px;
            font-weight: 600;
            cursor: pointer;
        }
        
        .btn:hover {
            background: #fff;
            color: #05B084;
        }
        
        .btn:active {
            background: #05B084;
            color: #ffffff;
        }
        
        .btn:disabled {
            opacity: 0.5;
            cursor: not-allowed;
        }
        
        .btn-secondary {
            background: #ffffff;
            color: #05B084;
            border: 1px solid #05B084;
        }
        
        .btn-secondary:hover {
            background: #05B084;
            color: #ffffff;
        }
        
        .loading {
            display: none;
            text-align: center;
            padding: 20px;
            color: #161616;
        }
        
        .loading.show {
            display: block;
        }
        
        .spinner {
            border: 3px solid #F0F0F0;
            border-top: 3px solid #05B084;
            border-radius: 50%;
            width: 40px;
            height: 40px;
            animation: spin 1s linear infinite;
            margin: 0 auto 10px;
        }
        
        @keyframes spin {
            0% { transform: rotate(0deg); }
            100% { transform: rotate(360deg); }
        }
        
        .networks {
            max-height: 300px;
            overflow-y: auto;
            margin-bottom: 16px;
            border: 1px solid #D0D0D0;
        }
        
        .network {
            padding: 10px;
            border-bottom: 1px solid #D0D0D0;
            cursor: pointer;
            display: flex;
            justify-content: space-between;
            align-items: center;
            background: #ffffff;
        }
        
        .network:last-child {
            border-bottom: none;
        }
        
        .network:hover {
            background: #FFF4E6;
        }
        
        .network.selected {
            background: #fff;
            color: #05B084;
            font-weight: 600;
        }
        
        .network-name {
            font-size: 14px;
        }
        
        .network-signal {
            font-size: 12px;
        }
        
        .info {
            background: #FFF4E6;
            padding: 12px;
            border: 1px solid #E0E0E0;
            margin-top: 16px;
            font-size: 12px;
            color: #161616;
            line-height: 1.5;
        }
        
        .warning {
            background: #FFF4E6;
            padding: 12px;
            border: 1px solid #FFA500;
            margin-bottom: 16px;
            font-size: 12px;
            color: #161616;
            line-height: 1.5;
        }
        
        .checkbox-label {
            display: flex;
            align-items: center;
            margin-top: 8px;
            font-size: 13px;
            color: #161616;
            cursor: pointer;
            user-select: none;
        }
        
        .checkbox-label input[type="checkbox"] {
            margin-right: 6px;
            cursor: pointer;
        }
        
        .checkbox-label span {
            cursor: pointer;
        }
        
        /* Modal Styles */
        .modal {
            display: none;
            position: fixed;
            z-index: 1000;
            left: 0;
            top: 0;
            width: 100%;
            height: 100%;
            background-color: rgba(0,0,0,0.5);
        }
        
        .modal.show {
            display: flex;
            align-items: center;
            justify-content: center;
        }
        
        .modal-content {
            background-color: #ffffff;
            border: 1px solid #E0E0E0;
            width: 90%;
            max-width: 400px;
            box-shadow: 0 4px 6px rgba(0,0,0,0.1);
        }
        
        .modal-header {
            background: #05B084;
            color: #fff;
            padding: 15px 20px;
            font-size: 16px;
            font-weight: 600;
        }
        
        .modal-body {
            padding: 20px;
        }
        
        .modal-footer {
            padding: 15px 20px;
            border-top: 1px solid #E0E0E0;
            display: flex;
            gap: 10px;
        }
        
        .pin-input {
            width: 100%;
            padding: 12px;
            border: 2px solid #D0D0D0;
            font-size: 18px;
            text-align: center;
            letter-spacing: 10px;
            background: #ffffff;
            color: #161616;
        }
        
        .pin-input:focus {
            outline: none;
            border-color: #05B084;
        }
        
        .error-msg {
            color: #D32F2F;
            font-size: 12px;
            margin-top: 8px;
            display: none;
        }
        
        .error-msg.show {
            display: block;
        }
        
        .hidden {
            display: none !important;
        }
        
        .modal-info {
            font-size: 13px;
            color: #666;
            margin-bottom: 15px;
            line-height: 1.5;
        }
        </style>
        </head>
        <body>
        
        <!-- PIN Verification Modal -->
        <div id="pinModal" class="modal">
            <div class="modal-content">
                <div class="modal-header">
                    Enter PIN
                </div>
                <div class="modal-body">
                    <p class="modal-info">Enter the 4-digit PIN to access the configuration portal</p>
                    <input type="password" id="pinInput" class="pin-input" maxlength="4" placeholder="****" inputmode="numeric" pattern="[0-9]*">
                    <p id="pinError" class="error-msg"></p>
                </div>
                <div class="modal-footer">
                    <button class="btn" onclick="verifyPIN()">Verify</button>
                </div>
            </div>
        </div>
        
        <!-- Update PIN Modal -->
        <div id="updatePinModal" class="modal">
            <div class="modal-content">
                <div class="modal-header">
                    Update PIN
                </div>
                <div class="modal-body">
                    <div class="warning">
                        <strong>Security Warning:</strong> You are now using the default PIN. To be more secure, please update your PIN.
                    </div>
                    <p class="modal-info">Enter a new 4-digit PIN</p>
                    <div class="form-group">
                        <label class="label">New PIN</label>
                        <input type="password" id="newPinInput" class="pin-input" maxlength="4" placeholder="****" inputmode="numeric" pattern="[0-9]*">
                    </div>
                    <div class="form-group">
                        <label class="label">Confirm</label>
                        <input type="password" id="confirmPinInput" class="pin-input" maxlength="4" placeholder="****" inputmode="numeric" pattern="[0-9]*">
                    </div>
                    <p id="updatePinError" class="error-msg"></p>
                </div>
                <div class="modal-footer">
                    <button class="btn btn-secondary" onclick="skipUpdatePIN()">Skip</button>
                    <button class="btn" onclick="updatePIN()">Update</button>
                </div>
            </div>
        </div>
        
        <div class="container">
        <div class="header">
        <h1>nuba_wm</h1>
        <p>A secure and simple WiFi manager with minimalist and modern UI</p>
        </div>
        <div class="content">
        <div id="scanSection">
        <button class="btn" onclick="scan()">Scan Networks</button>
        <div id="networks" class="networks" style="display:none"></div>
        </div>
        <br>
        
        <form id="wifiForm" onsubmit="save(event)">
        <div class="form-group">
        <label class="label">SSID</label>
        <input type="text" id="ssid" class="input" placeholder="Enter SSID" required maxlength="31">
        </div>
        <div class="form-group">
        <label class="label">Password</label>
        <input type="password" id="pass" class="input" placeholder="Enter password" maxlength="63">
        <label class="checkbox-label">
        <input type="checkbox" id="showPass" onclick="togglePassword()">
        <span>Show password</span>
        </label>
        </div>
        <button type="submit" class="btn" id="saveBtn">Connect</button>
        </form>
        
        <div class="loading" id="loading">
        <div class="spinner"></div>
        <p>Connecting to network...</p>
        </div>
        
        <div class="info">
        The device will reboot and try to connect to the WiFi network after you click the Connect button.
        </div>
        </div>
        </div>
        
        <script>
        let sel = null;
        let authenticated = false;
        let isDefaultPin = true;
        
        // Initialize on page load
        window.onload = function() {
            checkPINStatus();
        };
        
        function checkPINStatus() {
            fetch('/check-pin')
            .then(r => r.json())
            .then(d => {
                authenticated = d.authenticated;
                isDefaultPin = d.isDefault;
                
                if (!authenticated) {
                    showPINModal();
                } else if (isDefaultPin) {
                    showUpdatePINModal();
                }
            })
            .catch(e => {
                console.error('Failed to check PIN status:', e);
                showPINModal();
            });
        }
        
        function showPINModal() {
            document.getElementById('pinModal').classList.add('show');
            document.getElementById('pinInput').focus();
        }
        
        function hidePINModal() {
            document.getElementById('pinModal').classList.remove('show');
            document.getElementById('pinInput').value = '';
            document.getElementById('pinError').classList.remove('show');
        }
        
        function showUpdatePINModal() {
            document.getElementById('updatePinModal').classList.add('show');
            document.getElementById('newPinInput').focus();
        }
        
        function hideUpdatePINModal() {
            document.getElementById('updatePinModal').classList.remove('show');
            document.getElementById('newPinInput').value = '';
            document.getElementById('confirmPinInput').value = '';
            document.getElementById('updatePinError').classList.remove('show');
        }
        
        function verifyPIN() {
            const pin = document.getElementById('pinInput').value;
            const errorEl = document.getElementById('pinError');
            
            if (pin.length !== 4) {
                errorEl.textContent = 'PIN must be 4 digits';
                errorEl.classList.add('show');
                return;
            }
            
            fetch('/verify-pin', {
                method: 'POST',
                headers: {'Content-Type': 'application/x-www-form-urlencoded'},
                body: 'pin=' + encodeURIComponent(pin)
            })
            .then(r => r.json())
            .then(d => {
                if (d.success) {
                    authenticated = true;
                    isDefaultPin = d.isDefault;
                    hidePINModal();
                    
                    if (isDefaultPin) {
                        showUpdatePINModal();
                    }
                } else {
                    errorEl.textContent = 'Invalid PIN. Please try again.';
                    errorEl.classList.add('show');
                    document.getElementById('pinInput').value = '';
                    document.getElementById('pinInput').focus();
                }
            })
            .catch(e => {
                errorEl.textContent = 'Connection failed. Please try again.';
                errorEl.classList.add('show');
            });
        }
        
        function updatePIN() {
            const newPin = document.getElementById('newPinInput').value;
            const confirmPin = document.getElementById('confirmPinInput').value;
            const errorEl = document.getElementById('updatePinError');
            
            if (newPin.length !== 4 || confirmPin.length !== 4) {
                errorEl.textContent = 'PIN must be 4 digits';
                errorEl.classList.add('show');
                return;
            }
            
            if (newPin !== confirmPin) {
                errorEl.textContent = 'PINs do not match';
                errorEl.classList.add('show');
                return;
            }
            
            if (newPin === '0000') {
                errorEl.textContent = 'Cannot use default PIN (0000)';
                errorEl.classList.add('show');
                return;
            }
            
            fetch('/update-pin', {
                method: 'POST',
                headers: {'Content-Type': 'application/x-www-form-urlencoded'},
                body: 'newPin=' + encodeURIComponent(newPin)
            })
            .then(r => r.json())
            .then(d => {
                if (d.success) {
                    alert('PIN UPDATED SUCCESSFULLY\n\n • Remember your new PIN: ' + newPin + '\n');
                    isDefaultPin = false;
                    hideUpdatePINModal();
                } else {
                    errorEl.textContent = d.message || 'Failed to update PIN';
                    errorEl.classList.add('show');
                }
            })
            .catch(e => {
                errorEl.textContent = 'Connection failed. Please try again.';
                errorEl.classList.add('show');
            });
        }
        
        function skipUpdatePIN() {
            if (confirm('SECURITY WARNING\n\n • You are still use the default PIN.\n • Anyone can access and change the configuration without your knowledge.\n Are you sure about the risk?')) {
                hideUpdatePINModal();
            }
        }
        
        // Allow Enter key to submit PIN
        document.addEventListener('keypress', function(e) {
            if (e.key === 'Enter') {
                if (document.getElementById('pinModal').classList.contains('show')) {
                    verifyPIN();
                } else if (document.getElementById('updatePinModal').classList.contains('show')) {
                    updatePIN();
                }
            }
        });
        
        // Only allow numeric input for PIN fields
        document.querySelectorAll('.pin-input').forEach(input => {
            input.addEventListener('input', function(e) {
                this.value = this.value.replace(/[^0-9]/g, '');
            });
        });
        
        function togglePassword() {
            const passInput = document.getElementById('pass');
            const checkbox = document.getElementById('showPass');
            passInput.type = checkbox.checked ? 'text' : 'password';
        }
        
        function scan() {
            if (!authenticated) {
                alert('Not authenticated. Please enter your PIN first.');
                return;
            }
            
            document.getElementById('saveBtn').disabled = true;
            
            fetch('/scan')
            .then(r => r.json())
            .then(d => {
                const c = document.getElementById('networks');
                c.innerHTML = '';
                c.style.display = 'block';
                
                d.networks.forEach(n => {
                    const div = document.createElement('div');
                    div.className = 'network';
                    div.innerHTML = '<span class="network-name">' + n.ssid + '</span><span class="network-signal">' + n.rssi + ' dBm</span>';
                    
                    div.onclick = function() {
                        document.querySelectorAll('.network').forEach(e => e.classList.remove('selected'));
                        div.classList.add('selected');
                        document.getElementById('ssid').value = n.ssid;
                        sel = n.ssid;
                    };
                    
                    c.appendChild(div);
                });
                
                document.getElementById('saveBtn').disabled = false;
            })
            .catch(e => {
                alert('Scan failed');
                document.getElementById('saveBtn').disabled = false;
            });
        }
        
        function save(e) {
            e.preventDefault();
            
            if (!authenticated) {
                alert('Not authenticated. Please enter your PIN first.');
                return;
            }
            
            const s = document.getElementById('ssid').value;
            const p = document.getElementById('pass').value;
            
            document.getElementById('wifiForm').style.display = 'none';
            document.getElementById('loading').classList.add('show');
            
            fetch('/save', {
                method: 'POST',
                headers: {'Content-Type': 'application/x-www-form-urlencoded'},
                body: 'ssid=' + encodeURIComponent(s) + '&pass=' + encodeURIComponent(p)
            })
            .then(r => r.json())
            .then(d => {
                if (d.success) {
                    setTimeout(function() {
                        window.location.href = 'http://' + s.toLowerCase().replace(/[^a-z0-9]/g, '') + '.local';
                    }, 5000);
                } else {
                    alert('Failed to save credentials');
                    location.reload();
                }
            })
            .catch(e => {
                alert('Connection failed. Please try again.');
                setTimeout(function() { location.reload(); }, 1000);
            });
        }
        </script>
    </body>
</html>)=====";
        
#endif