#pragma once
#include <string>
#include <vector>
#include "Bungie\BlamPlayers.hpp"
#include "Bungie\Math\RealVector3D.hpp"
#include "Patches\PlayerPropertiesExtension.hpp"

namespace Game::Armor
{
	class ArmorExtension : public Patches::Network::PlayerPropertiesExtension<Bungie::Players::PlayerCustomization>
	{
	protected:
		void BuildData(int playerIndex, Bungie::Players::PlayerCustomization *out) override;
		void ApplyData(int playerIndex, Bungie::Players::PlayerProperties *properties, const Bungie::Players::PlayerCustomization &data) override;
		void Serialize(Bungie::BitStream *stream, const Bungie::Players::PlayerCustomization &data) override;
		void Deserialize(Bungie::BitStream *stream, Bungie::Players::PlayerCustomization *out) override;
	};

	void RefreshUiPlayer();
	void LoadArmorPermutations();
	void UpdateUiPlayerModelArmor();
	void SetUiPlayerModelTransform(const Bungie::Math::RealVector3D* newPosition, const float* rotationAngle);
}