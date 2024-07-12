#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Unit // 추상 클래스 : 순수 가상 함수를 가지고 있는 클래스
{
private:
	string name;
	int atk;
	int hp;

	int posX = 0;
	int posY = 0;

public:
	Unit(string _name, int _atk, int _hp) // 생성자로 기본 정보 할당
	{
		name = _name;
		atk = _atk;
		hp = _hp;
	}

	virtual void Move(int x, int y)
	{
		cout << GetName() << "은 " << x << ", " << y << " 좌표로 이동하였다" << "  ";
		cout << "좌표 : " << GetPosX() << ", " << GetPosY() << " => ";

		SetPosX(x);
		SetPosY(y);
		cout << GetPosX() << ", " << GetPosY() << endl;
	}

	// 순수 가상 함수(pure virtual function)
	virtual void Attack(Unit* unit) = 0;
	virtual void SkillA(Unit* unit) = 0; // 모든 유닛은 최소 두개의 스킬이 주어진다.
	virtual void SkillB(Unit* unit) = 0;

	void Damage(int damage)
	{
		cout << GetName() << "은 " << damage << "의 데미지를 입었다.  ";
		cout << "체력 : " << GetHp() << " => ";

		SetHp(GetHp() - damage);
		cout << GetHp() << endl;

		if (GetHp() <= 0)
		{
			cout << GetName() << "은 쓰러졌다." << endl;
			Die();
		}
	}

	string GetName()
	{
		return name;
	}

	int GetAtk()
	{
		return atk;
	}
	void SetAtk(int atk)
	{
		this->atk = atk;
	}

	int GetHp()
	{
		return hp;
	}
	void SetHp(int hp)
	{
		this->hp = hp;
	}

	int GetPosX()
	{
		return posX;
	}
	void SetPosX(int posX)
	{
		this->posX = posX;
	}

	int GetPosY()
	{
		return posY;
	}
	void SetPosY(int posY)
	{
		this->posY = posY;
	}

	void Upgrade()
	{
		cout << GetName() << "의 공격력은 업그레이드 되었다." << "  ";
		cout << GetAtk() << " => ";

		SetAtk(GetAtk() + 1);
		cout << GetAtk() << endl;
	}

	virtual void ShowInfo()
	{
		cout << "이름 : " << name << "  " << "공격력 : " << GetAtk() << "  " << "체력 : " << GetHp() << "  ";
		cout << "현재 좌표 : " << posX << ", " << posY << endl;
	}

protected:
	void Die()
	{
		delete this;
	}
};

class Marin : public Unit // 실체화 : 가상클래스를 상속받아 자식클래스를 생성. 추상 클래스인 유닛을 상속 받아서 마린을 정의한다.
{
public:
	Marin(string _name, int _atk, int _hp) : Unit(_name, _atk, _hp) {}

	void Attack(Unit* unit) override // 순수 가상 함수는 전부 오버라이드 해야한다.
	{
		cout << GetName() << "은 " << unit->GetName() << "에게 공격을 시도하였다." << endl;
		unit->Damage(GetAtk());
	}

	void SkillA(Unit* unit) override
	{
		cout << "Active Stim Pack!!" << endl;
		Upgrade();
	}

	void SkillB(Unit* unit) override
	{
		cout << "Active Granade!!" << endl;
		cout << GetName() << "은 " << unit->GetName() << "에게 수류탄을 투척하였다." << endl;
		unit->Damage(GetAtk() + 5);
	}
};

class Medic : public Unit
{
public:
	Medic(string _name, int _atk, int _hp) : Unit(_name, _atk, _hp) {}

	void Attack(Unit* unit) override
	{
		cout << "이런.. 메딕은 공격이 불가능하다!" << endl;
	}

	void SkillA(Unit* unit) override
	{
		cout << "Active Heal!" << endl;
		cout << GetName() << "은 " << unit->GetName() << "을 치료하였다." << "  ";

		cout << unit->GetName() << "의 체력 : " << unit->GetHp() << " => ";
		unit->SetHp(unit->GetHp() + GetAtk()); // 해당 유닛의 체력이 메딕의 공격력만큼 더한다.
		cout << unit->GetHp() << endl;
	}

	void SkillB(Unit* unit) override
	{
		cout << "Active Cure" << endl;
		cout << GetName() << "은 " << unit->GetName() << "을 정화하였다." << "  ";
	}
};

class Commander
{
private:
	vector<Unit*> unitGroup;

public:
	enum Command { GroupAttack, GroupMove };

	void AddGroup(Unit* unit)
	{
		unitGroup.push_back(unit);
	}

	void RemoveGroup(Unit* unit) // unitGroup에 지우고자 유닛이 있는지 확인하고 지운다.
	{
		vector<Unit*>::iterator it;

		for (it = unitGroup.begin(); it != unitGroup.end(); it++)
		{
			if (&unit == &(*it)) // 특정 지정한 유닛의 주소를 지정하고 unitGroup을 순회하며 각 유닛의 주소를 확인하고 처음 지정한 주소와 일치하면 break한다.(주소가 같다는건 내가 찾고자 하는 유닛을 의미한다)
				break;
		}

		if (it != unitGroup.end()) // break가 실행되었으면 해당 조건이 참(찾고자 하는 유닛이 vector에 있다), 실행되지 않았다면 해당 조건은 거짓이 된다(찾고자 하는 유닛이 vector에 없다) 
		{
			unitGroup.erase(it);
		}
	}

	void Command(Command command, Unit* unit, int moveX = 0, int moveY = 0)
	{
		switch (command)
		{
		case Command::GroupAttack:
			for (Unit* selectUnit : unitGroup)
			{
				selectUnit->Attack(unit);
			}
			break;

		case Command::GroupMove:
			for (Unit* selectUnit : unitGroup)
			{
				selectUnit->Move(moveX, moveY);
			}
			break;
		}
	}
};

int main()
{
	Commander commander;
	Marin* marinA = new Marin("강력한 마린", 3, 5);
	Marin* marinB = new Marin("타겟 마린", 2, 3);
	Marin* marinC = new Marin("또다른 마린", 4, 8);
	Marin* marinD = new Marin("단단한 마린", 2, 20);
	Medic* medicA = new Medic("야근하는 메딕", 3, 10);

	marinA->ShowInfo();
	marinA->Attack(marinB);

	marinA->Move(3, 3);
	marinA->SkillA(marinA);
	marinA->SkillB(marinC);

	medicA->Attack(marinD);
	marinA->Attack(marinD);

	medicA->SkillA(marinD);
	medicA->SkillB(marinD);

	Marin* marins[3];
	Medic* medics[2];

	for (int i = 0; i < size(marins); i++)
	{
		marins[i] = new Marin("마린 " + to_string(i + 1), 5, 3);
		commander.AddGroup(marins[i]);
	}

	for (int i = 0; i < size(medics); i++)
	{
		medics[i] = new Medic("메딕 " + to_string(i + 1), 0, 8);
		commander.AddGroup(medics[i]);
	}

	commander.Command(Commander::GroupAttack, marinD);
	commander.Command(Commander::GroupMove, NULL, 4, 2);

	return 0;
}