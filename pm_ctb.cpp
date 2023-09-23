#ifndef PROCESSMONITORING_PROCESSMONITORING
#define PROCESSMONITORING_PROCESSMONITORING

#ifdef _VISUAL_STUDIO_WORKSPACE
#include "include/program/cta.h"
#else
#include "include/program/cta.h"
#endif

#ifdef _VISUAL_STUDIO_WORKSPACE
#include "include/program/ctb.h"
#else
#include "include/program/ctb.h"
#endif

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