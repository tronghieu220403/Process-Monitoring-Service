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

#define _CTA

#ifdef _CTA

int main()
{
	auto cta = std::make_shared<pm::CTA>();
	std::jthread connection_thread(&pm::CTA::CommunicateWithCtb, cta);
	cta->Monitoring();

	return 0;
}

#endif

#endif