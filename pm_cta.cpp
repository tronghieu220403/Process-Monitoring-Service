#ifndef PROCESSMONITORING_PROCESSMONITORING
#define PROCESSMONITORING_PROCESSMONITORING

#include "program/cta.h"
#include "program/ctb.h"

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