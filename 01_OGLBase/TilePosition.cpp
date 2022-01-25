#include "TilePosition.h"

TilePosition::TilePosition(int lineIndex, int columnIndex) {
    setLineIndex(lineIndex);
    setColumnIndex(columnIndex);
}

int TilePosition::getLineIndex() const
{
    return lineIndex;
}

void TilePosition::setLineIndex(int lineIndex)
{
    this->lineIndex = lineIndex;
}

int TilePosition::getColumnIndex() const
{
    return columnIndex;
}

void TilePosition::setColumnIndex(int columnIndex)
{
    this->columnIndex = columnIndex;
}

