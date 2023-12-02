import serial
import re
import sys
import select

# Inicia conexión serial
ser = serial.Serial('/dev/ttyACM0', 115200)

# Expresión regular para capturar las etiquetas y valores
pattern = re.compile(r"(\w+): (\d+\.\d+)")

# Variables globales para almacenar la última nota detectada y su probabilidad
last_detected_note = None
max_probability = 0.0

# Función para procesar las etiquetas detectadas
def process_detected_labels(current_label, probability):
    global last_detected_note, max_probability

    # Elimina los últimos dos caracteres ' pr' de la etiqueta
    cleaned_label = current_label.strip()[:-2]

    # Verifica si la probabilidad supera el umbral y es mayor que la actual máxima probabilidad
    if probability > 0.87 and probability > max_probability:
        last_detected_note = cleaned_label
        max_probability = probability

# Espera por datos
print("Esperando datos... Presiona Enter para salir.")

# Procesa las líneas que vienen del serial
while True:
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').rstrip()
        print(f"{line}")  # Visualizar datos de entrada

        # Busca coincidencias en la línea actual
        matches = pattern.findall(line)

        # Procesa todas las coincidencias que superan el umbral
        for match in matches:
            label, value = match
            probability = float(value)

            # Procesa la nota detectada
            process_detected_labels(label, probability)

        # Toma decisiones basadas en la nota detectada con la mayor probabilidad
        #if last_detected_note is not None:
         #   print(f"Nota detectada con mayor probabilidad: {last_detected_note}")

    # Verifica si se ha presionado Enter para salir del bucle
    if sys.stdin in select.select([sys.stdin], [], [], 0)[0]:
        line = input()
        if line == '':
            break
