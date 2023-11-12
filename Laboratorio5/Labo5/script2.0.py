import serial
import re

# Inicia conexión serial
ser = serial.Serial('/dev/ttyACM0', 115200)

# Expresión regular para capturar las etiquetas y valores
pattern = re.compile(r"(\w+pr): (\d+\.\d+)")

# Variables globales para almacenar la última palabra detectada y su probabilidad
last_detected_word = None
max_probability = 0.0

# Función para procesar las etiquetas detectadas
def process_detected_labels(labels, current_label, probability):
    global last_detected_word, max_probability

    # Elimina los últimos dos caracteres 'pr' de la etiqueta
    cleaned_label = current_label[:-2]

    # Verifica si la probabilidad supera el umbral
    if probability > 0.90:
        labels.append(cleaned_label)

        # Actualiza la última palabra y su probabilidad si es la más alta hasta ahora
        if probability > max_probability:
            last_detected_word = cleaned_label
            max_probability = probability

# Espera por datos
print("Esperando datos...")

# Procesa las líneas que vienen del serial
while True:
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').rstrip()
        print(f"Dato recibido: {line}")  # Visualizar datos de entrada

        # Busca coincidencias en la línea actual
        matches = pattern.findall(line)

        # Muestra todas las coincidencias que superan el umbral
        detected_labels = []
        for match in matches:
            label, value = match
            probability = float(value)

            # Procesa las etiquetas detectadas
            process_detected_labels(detected_labels, label, probability)

        # Toma decisiones basadas en las etiquetas detectadas
        if detected_labels:
            print(f"Palabras detectadas: {detected_labels}")

# Imprime la última palabra detectada y su probabilidad al salir del bucle
print(f"Última palabra detectada: {last_detected_word} con probabilidad: {max_probability}")
