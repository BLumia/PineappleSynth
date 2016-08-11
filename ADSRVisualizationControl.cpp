#include "ADSRVisualizationControl.h"


double ADSRVisualizationControl::convertToGraphicX(double value) {
	double min = (double) this->mRECT.L;
	double distance = (double) this->mRECT.W();
	return value * distance + min;
};
double ADSRVisualizationControl::convertToPercentX(double value) {
	double min = (double) this->mRECT.L;
	double position = value - min;
	double distance = (double) this->mRECT.W();
	return position / distance;
};
double ADSRVisualizationControl::convertToGraphicY(double value) {
	double min = (double) this->mRECT.T;
	double distance = (double) this->mRECT.H();
	// We use (1 - value) as the max value 1 is located on top of graphics and not bottom
	return (1 - value) * distance + min;
};
double ADSRVisualizationControl::convertToPercentY(double value) {
	double min = (double) this->mRECT.T;
	double position = value - min;
	double distance = (double) this->mRECT.H();
	// We return the 1 - distance as the value 1 is located on top of graphics and not bottom
	return 1 - position / distance;
};

bool ADSRVisualizationControl::Draw(IGraphics *pGraphics) {
	IColor color(255, 50, 200, 20);

	//Cycles
	pGraphics->DrawCircle(&color, convertToGraphicX(startPoint.x), convertToGraphicY(startPoint.y), 3, 0, true);
	pGraphics->DrawCircle(&color, convertToGraphicX(attackPoint.x), convertToGraphicY(attackPoint.y), 3, 0, true);
	pGraphics->DrawCircle(&color, convertToGraphicX(retainPoint.x), convertToGraphicY(retainPoint.y), 3, 0, true);
	pGraphics->DrawCircle(&color, convertToGraphicX(endPoint.x), convertToGraphicY(endPoint.y), 3, 0, true);

	//Lines
	pGraphics->DrawLine(&color,
		convertToGraphicX(startPoint.x), convertToGraphicY(startPoint.y),
		convertToGraphicX(attackPoint.x), convertToGraphicY(attackPoint.y),
		0, true);
	pGraphics->DrawLine(&color,
		convertToGraphicX(attackPoint.x), convertToGraphicY(attackPoint.y),
		convertToGraphicX(retainPoint.x), convertToGraphicY(retainPoint.y),
		0, true);
	pGraphics->DrawLine(&color,
		convertToGraphicX(retainPoint.x), convertToGraphicY(retainPoint.y),
		convertToGraphicX(endPoint.x), convertToGraphicY(endPoint.y),
		0, true);

	return true;
};

void ADSRVisualizationControl::initPoints() {
	setADSR(0.01, 0.5, 0.1, 1.0);
}

//Four stage with different max value, and make it to [0,1]
void ADSRVisualizationControl::setADSR(float att, float dec, float sus, float rel) {
	attack = att;//ToNormalizedParam(att / 10, 0, 1, 3);
	decay = dec;//ToNormalizedParam(dec / 15, 0, 1, 3);
	sustain = sus;// ToNormalizedParam(sus, 0, 1, 2);
	release = rel;// ToNormalizedParam(rel / 15, 0, 1, 3);
	updatePointPosition();
}

void ADSRVisualizationControl::updatePointPosition() {
	//if add a delay stage, should change this.
	startPoint.x = 0;
	startPoint.y = 0.1;
	//accoriding to attack value, div 3 in order to let it in the currect pos;
	attackPoint.x = attack / 3 + startPoint.x;
	attackPoint.y = 1;
	//accroiding to decay value and sustain value
	retainPoint.x = decay / 3 + attackPoint.x;
	retainPoint.y = sustain * 0.9 + 0.1;
	//end value
	endPoint.x = retainPoint.x + release / 3;
	endPoint.y = 0.1;
	//set dirty
	SetDirty();
}