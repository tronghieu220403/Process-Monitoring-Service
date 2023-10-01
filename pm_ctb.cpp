#ifndef PROCESSMONITORING_PROCESSMONITORING
#define PROCESSMONITORING_PROCESSMONITORING

#include "program/cta.h"
#include "program/ctb.h"

#define _CTB

#ifdef _CTB

#include <iostream>

int main()
{
	pm::CTB ctb;
	ctb.AddToStartUp();
	ctb.CommunicateWithCta();
	return 0;
}

#endif

#endif