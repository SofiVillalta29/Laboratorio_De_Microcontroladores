import serial
import re
import sys
import select

# Inicia conexión serial
ser = serial.Serial('/dev/ttyACM0', 115200)

# Expresión regular para capturar las etiquetas y valores
pattern = re.compile(r"(\w+): (\d+\.\d+)")

# Variables globales para almacenar la última palabra detectada y su probabilidad
last_detected_word = None
max_probability = 0.0

# Lista para almacenar todas las palabras detectadas
all_detected_words = []

# Función para procesar las etiquetas detectadas
def process_detected_labels(labels, current_label, probability):
    global last_detected_word, max_probability

    # Elimina los últimos dos caracteres 'pr' de la etiqueta
    cleaned_label = current_label[:-2]

    # Verifica si la probabilidad supera el umbral
    if probability > 0.87:
        labels.append(cleaned_label)

        # Actualiza la última palabra y su probabilidad si es la más alta hasta ahora
        if probability > max_probability:
            last_detected_word = cleaned_label
            max_probability = probability

# Espera por datos
print("Esperando datos... Presiona Enter para salir.")

# Procesa las líneas que vienen del serial
while True:
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').rstrip()
        print(f"Dato de Cuerda detectada: {line}")  # Visualizar datos de entrada

        # Busca coincidencias en la línea actual
        matches = pattern.findall(line)

        # Muestra todas las coincidencias que superan el umbral
        detected_labels = []
        for match in matches:
            label, value = match
            probability = float(value)

            # Procesa las cuerdas detectadas
            process_detected_labels(detected_labels, label, probability)

        # Toma decisiones basadas en las cuerdas detectadas
        if detected_labels:
            print(f"Palabras detectadas: {detected_labels}")

            # Agrega las cuerdas detectadas a la lista global
            all_detected_words.extend(detected_labels)

    # Verifica si se ha presionado Enter para salir del bucle
    if sys.stdin in select.select([sys.stdin], [], [], 0)[0]:
        line = input()
        if line == '':
            break
 