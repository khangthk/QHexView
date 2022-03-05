#pragma once

#include <QUndoStack>
#include "buffer/qhexbuffer.h"
#include "qhexmetadata.h"
#include "qhexoptions.h"

class QHexCursor;
class QPalette;

class QHexDocument: public QObject
{
    Q_OBJECT

    public:
        enum class FindDirection { Forward, Backward };

    private:
        explicit QHexDocument(QHexBuffer* buffer, const QHexOptions& options, QObject *parent = nullptr);
        void checkOptions(QPalette palette);
        qint64 getLastColumn(qint64 line) const;
        qint64 lastLine() const;
        quint64 lines() const;

    public:
        bool isEmpty() const;
        bool canUndo() const;
        bool canRedo() const;
        QHexCursor* cursor() const;
        const QHexOptions* options() const;
        QHexMetadata* metadata() const;
        qint64 length() const;
        quint64 baseAddress() const;
        qint64 find(const QByteArray &ba, FindDirection fd = FindDirection::Forward) const;
        QByteArray getLine(qint64 line) const;
        QByteArray read(qint64 offset, int len = 0) const;
        uchar at(int offset) const;

    public Q_SLOTS:
        void undo();
        void redo();
        void cut(bool hex = false);
        void copy(bool hex = false);
        void paste(bool hex = false);
        void selectAll();
        void insert(qint64 offset, uchar b);
        void replace(qint64 offset, uchar b);
        void insert(qint64 offset, const QByteArray& data);
        void replace(qint64 offset, const QByteArray& data);
        void remove(qint64 offset, int len);
        bool saveTo(QIODevice* device);
        void setLineLength(unsigned int l);
        void setGroupLength(unsigned int l);
        void setScrollSteps(unsigned int l);
        void setBaseAddress(quint64 baseaddress);
        void setOptions(const QHexOptions& options);
        void setByteColor(quint8 b, QHexColor c);
        void setByteForeground(quint8 b, QColor c);
        void setByteBackground(quint8 b, QColor c);

    public:
        template<typename T, bool Owned = true> static QHexDocument* fromDevice(QIODevice* iodevice, const QHexOptions& options = { }, QObject* parent = nullptr);
        template<typename T> static QHexDocument* fromMemory(char *data, int size, const QHexOptions& options = { }, QObject* parent = nullptr);
        template<typename T> static QHexDocument* fromMemory(const QByteArray& ba, const QHexOptions& options = { }, QObject* parent = nullptr);
        static QHexDocument* fromLargeFile(QString filename, const QHexOptions& options = { }, QObject *parent = nullptr);
        static QHexDocument* fromFile(QString filename, const QHexOptions& options = { }, QObject* parent = nullptr);
        static QHexDocument* create(const QHexOptions& options = { }, QObject* parent = nullptr);

    Q_SIGNALS:
        void canUndoChanged(bool canUndo);
        void canRedoChanged(bool canRedo);
        void changed();

    private:
        QHexOptions m_options;
        QHexMetadata* m_hexmetadata;
        QHexCursor* m_hexcursor;
        QHexBuffer* m_buffer;
        QUndoStack m_undostack;
        quint64 m_baseaddress;

    friend class QHexView;
};

template<typename T, bool Owned>
QHexDocument* QHexDocument::fromDevice(QIODevice* iodevice, const QHexOptions& options, QObject *parent)
{
    QHexBuffer* hexbuffer = new T(parent);
    if(Owned) iodevice->setParent(hexbuffer);
    return hexbuffer->read(iodevice) ? new QHexDocument(hexbuffer, options, parent) : nullptr;
}

template<typename T>
QHexDocument* QHexDocument::fromMemory(char *data, int size, const QHexOptions& options, QObject *parent)
{
    QHexBuffer* hexbuffer = new T();
    hexbuffer->read(data, size);
    return new QHexDocument(hexbuffer, options, parent);
}

template<typename T>
QHexDocument* QHexDocument::fromMemory(const QByteArray& ba, const QHexOptions& options, QObject *parent)
{
    QHexBuffer* hexbuffer = new T();
    hexbuffer->read(ba);
    return new QHexDocument(hexbuffer, options, parent);
}
