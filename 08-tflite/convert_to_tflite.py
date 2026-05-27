import numpy as np
import tensorflow as tf

MODEL_PATH = "model_keras_small_gray.h5"
CALIB_PATH = "calib_data_grayscale.npy"
OUTPUT_PATH = "small_gray.tflite"

print("Cargando modelo...")
model = tf.keras.models.load_model(MODEL_PATH)
model.summary()

print("\nCargando datos de calibracion...")
calib_data = np.load(CALIB_PATH)
print(f"Shape calib_data: {calib_data.shape}")
print(f"Dtype calib_data: {calib_data.dtype}")
print(f"Min: {calib_data.min():.4f} Max: {calib_data.max():.4f}")

def representative_dataset():
    for i in range(len(calib_data)):
        sample = calib_data[i]
        sample = sample.astype(np.float32)
        sample = np.expand_dims(sample, axis=0)
        yield [sample]

print("\nConfigurando conversor TFLite...")
converter = tf.lite.TFLiteConverter.from_keras_model(model)
converter.optimizations = [tf.lite.Optimize.DEFAULT]
converter.representative_dataset = representative_dataset
converter.target_spec.supported_ops = [tf.lite.OpsSet.TFLITE_BUILTINS_INT8]
converter.inference_input_type = tf.float32
converter.inference_output_type = tf.float32

print("Convirtiendo...")
tflite_model = converter.convert()

with open(OUTPUT_PATH, "wb") as f:
    f.write(tflite_model)

print(f"\nModelo guardado en {OUTPUT_PATH}")
print(f"Tamaño original (.h5): {tf.io.gfile.GFile(MODEL_PATH).size() / 1024:.1f} KB")
print(f"Tamaño convertido (.tflite): {len(tflite_model) / 1024:.1f} KB")
