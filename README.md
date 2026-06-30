# PoketStage

Proyecto inicial para un gadget de setlist para músicos de cuerda eléctrica con ESP32-C3, pantalla OLED SSD1306 y 3 botones.

## Qué hace ahora

- Muestra una setlist de ejemplo en una pantalla OLED SSD1306.
- Permite navegar entre canciones con tres botones.
- Muestra nombre, afinación y pedales recomendados por canción.
- Ajusta automáticamente el texto largo para que no tape la parte inferior de la pantalla.

## Estructura del proyecto

- src/main.cpp: lógica principal, navegación y render de pantallas
- src/models/Song.h: modelo de datos de la canción
- src/data/ExampleSetlist.h y ExampleSetlist.cpp: setlist de ejemplo en memoria
- src/ (futuro): agregar JSON loader, UI helpers, storage y networking

## Conexiones actuales

- Botón izquierdo: GPIO 0
- Botón central: GPIO 1
- Botón derecho: GPIO 2
- SDA OLED: GPIO 8
- SCL OLED: GPIO 9

> Los botones usan INPUT_PULLUP, así que se activan al llevar el pin a LOW.

## Próximos pasos

1. Reemplazar la setlist embebida por carga desde JSON.
2. Añadir almacenamiento persistente para favoritos o setlists.
3. Conectar un backend para sugerir afinaciones y pedales.
4. Mantener la API key fuera del firmware.
