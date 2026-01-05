from flask import Flask, request, jsonify
from flask_cors import CORS
import serial

app = Flask(__name__)
CORS(app)  # abilita CORS

# Porta seriale (modifica se necessario)
ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)

@app.route('/move', methods=['POST'])
def move():
    data = request.get_json()
    angolo1 = data.get('angolo1')
    angolo2 = data.get('angolo2')

    comando = f"{angolo1},{angolo2}\n"
    ser.write(comando.encode())
    print("Inviato:", comando)

    return jsonify({"status": "ok", "angolo1": angolo1, "angolo2": angolo2})

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)
