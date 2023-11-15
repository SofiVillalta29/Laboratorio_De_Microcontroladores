import paho.mqtt.client as mqtt
import serial
import json
import re
import sys
import select

serial_port = serial.Serial(port="/dev/ttyACM0", baudrate=115200, timeout=1)

# Configuración MQTT para Thingsboard
broker = "iot.eie.ucr.ac.cr"
port = 1883
token = "wpkptsjr859rj2z10iqo"

username = "Labo5_Sofia_Elias"

client = mqtt.Client()
client.username_pw_set(token)
client.connect(broker, port, 60)
client.loop_start()

# Expresión regular para capturar las etiquetas y valores
pattern = re.compile(r"(\w+pr): (\d+\.\d+)")

# Variable global para almacenar la última palabra detectada
last_detected_word = None

def on_publish(client, userdata, result):
    print("data published to thingsboard")
    pass

client.on_publish = on_publish

# Definición de la función para procesar etiquetas detectadas
def process_detected_labels(current_label, probability):
    global last_detected_word

    # Elimina los últimos dos caracteres 'pr' de la etiqueta
    cleaned_label = current_label[:-2]

    # Verifica si la probabilidad supera el umbral
    if probability > 0.87:
        last_detected_word = cleaned_label

while True:
    if serial_port.in_waiting > 0:
        line = serial_port.readline().decode('utf-8').rstrip()
        print(f"Dato recibido: {line}")  # Visualizar datos de entrada

        # Busca coincidencias en la línea actual
        matches = pattern.findall(line)

        # Procesa las etiquetas detectadas
        for match in matches:
            label, value = match
            probability = float(value)
            process_detected_labels(label, probability)

        # Toma decisiones basadas en la última palabra detectada
        if last_detected_word:
            print(f"Última palabra detectada: {last_detected_word}")
            # Aquí puedes agregar la lógica para publicar la última palabra en MQTT
            message = {"word": last_detected_word}
            client.publish(f"v1/devices/me/telemetry", json.dumps(message), 1)

    # Verifica si se ha presionado Enter para salir del bucle
    if sys.stdin in select.select([sys.stdin], [], [], 0)[0]:
        line = input()
        if line == '':
            break

# Cerrar la conexión del puerto serial y detener el bucle del cliente MQTT
serial_port.close()
client.loop_stop()
