#pragma once

#include "Skill.h"

class EmpoweredSkill : public Skill
{
private:
	const double COOLDOWN = 10000;	//En milisegundos
	const double MANA_COST = 10;

public:
	EmpoweredSkill(Player* player) : Skill(player, SkillType::Active, SkillBranch::Physical) { costMana_ = MANA_COST; };
	virtual ~EmpoweredSkill() {};

	virtual void action() {
		double mana = player_->getMana();
		//Si no est� en cooldown la habilidad
		if (costMana_ <= mana && !skillCD_.isCooldownActive())
		{
			player_->removeMana(costMana_);
			player_->activateEmpowered();
			skillCD_.initCooldown(COOLDOWN);
		}
	};
};

