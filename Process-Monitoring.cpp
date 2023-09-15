#ifdef _VISUAL_STUDIO_WORKSPACE
#pragma once
#endif


#ifndef PROCESSMONITORING_PROCESSMONITORING
#define PROCESSMONITORING_PROCESSMONITORING

#ifdef _VISUAL_STUDIO_WORKSPACE
#include "src/CTA.h"
#else
#include "src/CTA.h"
#endif

#include <iostream> 
#include <vector>

#include <thread>

int main()
{
	pm::CTA cta;
	cta.AddToStartup();
	std::jthread connection_thread(&pm::CTA::CommunicateWithCtb,cta);
	cta.Monitoring();
	return 0;
}

#endif