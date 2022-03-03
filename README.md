<div align="center">
 <img src="https://user-images.githubusercontent.com/1503603/156148159-02d1181a-153e-4d0b-9512-5f9ac30bcd2f.png" alt="QHexView logo" title="QHexView"/>
</div>
<div align="center">
 <i>An hexadecimal widget for Qt5</i>
 <br>
 <br>
 <div align="center">
  <img src="https://user-images.githubusercontent.com/1503603/156016117-f11a6ccd-98ff-44eb-8127-b26c475f87f7.png" height="400"/>
  <br>
  <img src="https://img.shields.io/badge/license-MIT-8e725e.svg?style=flat-square">
  <a href="https://github.com/ellerbrock/open-source-badges/">
    <img src="https://badges.frapsoft.com/os/v1/open-source.png?v=103">
  </a>  
 </div>
</div>
    
Features
-----
- Document/View based
- Unlimited Undo/Redo
- Fully Customizable
- Fast rendering
- Easy to Use

Backends
-----
These are the available buffer backends:
- *QMemoryBuffer*: A simple, flat memory
- *QMemoryRefBuffer*: QHexView just display the referenced data, editing is disabled
- *QDeviceBuffer*: A read-only view for QIODevice 

It's also possible to create new data backends from scratch.

Usage
-----
Data is managed by QHexView through QHexDocument class.<br>
It's possible to load a generic QIODevice with QHexDocument's method `fromDevice()` with various buffer backends.<br>
Helper methods are provided in order to load a QFile as In-Memory buffer:<br>
```cpp
#include <qhexview.h>

QHexOptions options; // This is optional
options.grouplength = 2; // Pack bytes as AABB
options.bytecolors[0x00] = {Qt::lightGray, QColor()}; // Highlight '00's
options.bytecolors[0xFF] = {Qt::darkBlue, QColor()};  // Highlight 'FF's

/* Load data from In-Memory Buffer... */
QHexDocument* document = QHexDocument::fromMemory<QMemoryBuffer>(bytearray, options);
/* ...from a generic I/O device... */
QHexDocument* document = QHexDocument::fromDevice<QMemoryBuffer>(iodevice, options);
/* ...or from File... */
QHexDocument* document = QHexDocument::fromFile<QMemoryBuffer>("data.bin", options);

QHexView* hexview = new QHexView();
hexview->setDocument(document);                  // Associate QHexEditData with this QHexEdit (ownership is not changed)

// Document editing
QByteArray data = document->read(24, 78);        // Read 78 bytes starting to offset 24
document->insert(4, "Hello QHexEdit");           // Insert a string to offset 4 
document->remove(6, 10);                         // Delete bytes from offset 6 to offset 10 
document->replace(30, "New Data");               // Replace bytes from offset 30 with the string "New Data"

// Metatadata management (available from QHexView too)
QHexMetadata* hexmetadata = document->metadata();

hexmetadata->setBackground(5, 10, Qt::Red);         // Highlight background at offset range [5, 10)
hexmetadata->setForeground(15, 30, Qt::darkBLue);   // Highlight background at offset range [15, 30)
hexmetadata->setComment(12, 42, "I'm a comment!");  // Add a comment at offset range [12, 42)
hexmetadata->unhighlight();                         // Reset highlighting
hexmetadata->clear();                               // Reset all styles
```
