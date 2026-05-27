# 08 - TFLite Edge Inference

Inferencia de red neuronal corriendo directamente en la Raspberry Pi 3B+
usando TensorFlow Lite. El modelo detecta huevos parasitarios en imágenes
de microscopio — el mismo modelo del PFC pero deployado en hardware de $35.

## Qué hace
- Carga un modelo Keras (.h5) y lo convierte a TFLite con dynamic range quantization
- Recorre la imagen con una ventana deslizante de 15x15 píxeles (1344 parches)
- Clasifica cada parche como huevo o no huevo
- Reporta el conteo total y el tiempo de inferencia

## Resultados
- full_test.bmp → 94 huevos (referencia float32: 94) ✓
- Test2.bmp     → 99 huevos (referencia float32: 101)
- Tiempo de inferencia pura: ~0.5s
- Tiempo real total: ~14s (dominado por el import de TensorFlow)

## Conversión del modelo
```bash
python3 convert_to_tflite.py
```

Usa dynamic range quantization — los pesos quedan en INT8, entrada/salida
en float32. No se pudo usar full INT8 quantization porque el modelo tiene
una InputLayer explícita que TF 2.21 no calibra correctamente con el
generador representativo.

## Inferencia
```bash
python3 inference.py
```

## Dependencias
```bash
python3 -m pip install tensorflow Pillow --break-system-packages
```

tflite-runtime no tiene wheels para ARM64/Python 3.13, por lo que se
instaló TensorFlow completo como alternativa. Esto explica el tiempo
de startup elevado.

## Performance real

```
Métrica                    TFLite (este proyecto)    ONNX Runtime (09-onnx)
───────────────────────────────────────────────────────────────────────────
Inferencia pura            0.5s                      1.0s
Tiempo real total          14s                       2s
Overhead startup           13.5s                     1s
Tamaño modelo              57KB (INT8w)              192KB (float32)
```

TFLite gana en inferencia pura porque los pesos están cuantizados a INT8.
ONNX Runtime gana en tiempo real porque su runtime es mucho más liviano.

Para servicios continuos (systemd) TFLite es mejor — el startup se paga
una sola vez. Para scripts puntuales en campo, ONNX Runtime es más práctico.
