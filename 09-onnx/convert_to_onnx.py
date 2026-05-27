import tensorflow as tf
import tf2onnx
import onnx
import os

MODEL_PATH  = "model_keras_small_gray.h5"
OUTPUT_PATH = "small_gray.onnx"

print("Cargando modelo Keras...")
model = tf.keras.models.load_model(MODEL_PATH)
print(f"Input shape:  {model.input_shape}")
print(f"Output shape: {model.output_shape}")

print("\nConvirtiendo a ONNX...")
input_signature = [tf.TensorSpec(shape=(1, 15, 15, 1), dtype=tf.float32)]
model_proto, _ = tf2onnx.convert.from_keras(model, input_signature=input_signature, opset=13)

onnx.save(model_proto, OUTPUT_PATH)
print(f"Modelo guardado en {OUTPUT_PATH}")

print("\nVerificando modelo ONNX...")
model_onnx = onnx.load(OUTPUT_PATH)
onnx.checker.check_model(model_onnx)
print("Verificacion OK")

print(f"\nTamaño original (.h5):     {tf.io.gfile.GFile(MODEL_PATH).size() / 1024:.1f} KB")
print(f"Tamaño convertido (.onnx): {os.path.getsize(OUTPUT_PATH) / 1024:.1f} KB")
