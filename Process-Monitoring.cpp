#ifndef PROCESSMONITORING_PROCESSMONITORING
#define PROCESSMONITORING_PROCESSMONITORING

#ifdef _VISUAL_STUDIO_WORKSPACE
#include "src/CTA.h"
#else
#include "src/CTA.h"
#endif

int main()
{
	auto cta = std::make_shared<pm::CTA>();
	cta->AddToStartup();
	std::jthread connection_thread(&pm::CTA::CommunicateWithCtb, cta);
	cta->Monitoring();
	//std::jthread monitoring_thread(&pm::CTA::Monitoring, cta);

	return 0;
}


#endif