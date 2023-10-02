#pragma once

namespace
{
	extern int g_insertion_point;
}

namespace Patches::Campaign
{
	void ApplyAll();
	void SetInsertionPoint(int insertion);
};