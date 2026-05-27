import numpy as np
import tensorflow as tf
from PIL import Image
from time import time

MODEL_PATH = "small_gray.tflite"
IMAGE_PATH = "full_test.bmp"
PATCH_SIZE  = 15
THRESH      = 0.90

interpreter = tf.lite.Interpreter(model_path=MODEL_PATH)
interpreter.allocate_tensors()

input_details  = interpreter.get_input_details()
output_details = interpreter.get_output_details()

img       = Image.open(IMAGE_PATH).convert('L')
img_array = np.array(img, dtype=np.float32) / 255.0
h, w      = img_array.shape
print(f"Imagen: {w}x{h} px")

resultados = []
start = time()

for j in range(h // PATCH_SIZE):
    for i in range(w // PATCH_SIZE):
        patch = img_array[j*15:j*15+15, i*15:i*15+15].reshape(1, 15, 15, 1)

        interpreter.set_tensor(input_details[0]['index'], patch)
        interpreter.invoke()

        output = interpreter.get_tensor(output_details[0]['index'])
        res    = output[0]

        if res[0] > 0.3:
            res[1] = 0
        resultados.append(res[1])

stop = time()

huevos = sum(r > THRESH for r in resultados)
print(f"Huevos detectados: {huevos}")
print(f"Tiempo de inferencia: {stop - start:.4f}s")
