#pragma once
class TilePosition
{
private:
	int lineIndex;		//sor index
	int columnIndex;	//oszlop index

public:
    TilePosition(int lineIndex, int columnIndex);
    int getLineIndex() const;
    void setLineIndex(int lineIndex);
    int getColumnIndex() const;
    void setColumnIndex(int columnIndex);
};

