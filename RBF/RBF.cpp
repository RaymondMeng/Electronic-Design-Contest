#include "RBF.h"

RBF::RBF()
{
    this->Dataset.fill(0);
    this->Input.fill(0);
    this->Delta.fill(0);
    this->Norm_sqar.fill(0);
    this->Weight.fill(0.1);
    this->Center.fill(0);
}

