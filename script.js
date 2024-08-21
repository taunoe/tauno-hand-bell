// Replace with your actual Blynk Auth Token and Virtual Pin
const authToken = "MdlHJUkkWEBwuTIjYy4yQd6dY_Krrp5m";
const virtualPin = "V0";  // Replace with the actual virtual pin number

const baseUrl = `https://blynk.cloud/external/api/update?token=${authToken}&${virtualPin}`;

// Function to send a request to Blynk
function sendBlynkRequest(value) {
    const url = `${baseUrl}=${value}`;
    fetch(url)
        .then(response => {
                    if (response.ok) {
                        alert(`Virtual Pin ${virtualPin} set to ${value}`);
                    } else {
                        alert("Failed to send request to Blynk");
                    }
        })
        .catch(error => {
            alert("Error: " + error);
        });
}

// Event listeners for buttons
document.getElementById("onButton").onclick = function() {
    sendBlynkRequest(1);  // Send 1 to turn ON the virtual pin
};

document.getElementById("offButton").onclick = function() {
    sendBlynkRequest(0);  // Send 0 to turn OFF the virtual pin
};

