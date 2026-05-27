# 08 - TFLite Edge Inference

Inferencia de red neuronal corriendo directamente en la Raspberry Pi 3B+
usando TensorFlow Lite. El modelo detecta huevos parasitarios en imágenes
de microscopio.

## Qué hace
- Carga un modelo Keras (.h5) cuantizado a TFLite
- Recorre la imagen con una ventana deslizante de 15x15 píxeles (1344 parches)
- Clasifica cada parche como huevo o no huevo
- Reporta el conteo total y el tiempo de inferencia

## Resultados
- full_test.bmp → 94 huevos (referencia float32: 94) ✓
- Test2.bmp     → 99 huevos (referencia float32: 101)
- Tiempo de inferencia: ~0.42 segundos por imagen

## Conversión del modelo
El modelo se convierte en la misma Pi con el script `convert_to_tflite.py`:

```bash
python3 convert_to_tflite.py
```

Usa dynamic range quantization — los pesos quedan en INT8, entrada/salida en float32.

## Inferencia

```bash
python3 inference.py
```

## Dependencias

```bash
python3 -m pip install tensorflow Pillow --break-system-packages
```

## Contexto
El mismo modelo corría en la Ultra96-V2 con DPU B1600 via Vitis AI.
En la Pi tarda ~0.42s por imagen vs milisegundos en el DPU, pero para
la aplicación de campo (contar huevos de una muestra McMaster) el tiempo
es perfectamente aceptable con hardware más barato.
