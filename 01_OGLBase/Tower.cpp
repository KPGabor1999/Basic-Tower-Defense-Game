#include "Tower.h"

Tower::Tower(Level* level, int lineIndex, int columnIndex): GameObject(level, lineIndex, columnIndex){}

bool Tower::inRange(Enemy* enemy) const {
	//std::cout << "Jelenlegi pozicio: [" << tilePosition.lineIndex << "][" << tilePosition.columnIndex << "]" << std::endl;
	//std::cout << "Hatotavolsagok: " << zRangeStart << "   " << zRangeEnd << "   " << xRangeStart << "   " << xRangeEnd << std::endl << std::endl;

	if (enemyInSight(enemy)) {
		return true;
	}
	else {
		return false;
	}
}

bool Tower::enemyInSight(Enemy* enemy) const {
	return
		enemy->getLineIndex() >= zRangeStart &&
		enemy->getLineIndex() <= zRangeEnd &&
		enemy->getColumnIndex() >= xRangeStart &&
		enemy->getColumnIndex() <= xRangeEnd;
}