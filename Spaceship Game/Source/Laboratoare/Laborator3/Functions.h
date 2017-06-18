#pragma once

float computeAngle(float x1, float y1, float x2, float y2) {
	float distanceX = abs(x1 - x2);
	float distanceY = abs(y1 - y2);
	float angle = 0;

	if (x1 > x2 && y1 > y2) {
		angle = atan(distanceY / distanceX);
	}
	else if (x1 < x2 && y1 > y2) {
		angle = M_PI - atan(distanceY / distanceX);
	}
	else if (x1 < x2 && y1 < y2) {
		angle = M_PI + atan(distanceY / distanceX);
	}
	else if (x1 > x2 && y1 < y2) {
		angle = 3 * M_PI / 2 + atan(distanceX / distanceY);
	}

	return angle;
}

float computeDistance(float x1, float y1, float x2, float y2) {
	float distanceX = abs(x1 - x2);
	float distanceY = abs(y1 - y2);
	float distance = sqrt(distanceX * distanceX + distanceY * distanceY);

	return distance;
}