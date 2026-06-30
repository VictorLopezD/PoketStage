# PoketStage

Proyecto inicial para un gadget de setlist para músicos de cuerda eléctrica con ESP32-C3, pantalla OLED SSD1306 y 3 botones.

## Estructura recomendada

- src/main.cpp: lógica principal, navegación y render de pantallas
- src/models/Song.h: modelo de datos de la canción
- src/data/ExampleSetlist.h y ExampleSetlist.cpp: setlist de ejemplo en memoria
- src/ (futuro): agregar JSON loader, UI helpers, storage y networking

## Próximos pasos

1. Reemplazar la setlist embebida por carga desde JSON.
2. Añadir almacenamiento persistente para favoritos o setlists.
3. Conectar un backend para sugerir afinaciones y pedales.
4. Mantener la API key fuera del firmware.

## Mapeo de pines inicial

- Botón izquierdo: GPIO 4
- Botón central: GPIO 5
- Botón derecho: GPIO 6
- SDA OLED: GPIO 8
- SCL OLED: GPIO 9
