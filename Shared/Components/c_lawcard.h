#ifndef C_LAWCARD_H
#define C_LAWCARD_H

class C_LawCard
{
public:
    enum E_FACTION
    {
        notDefined,
        phenixOrder,
        deathEater
    };

public:
    C_LawCard(E_FACTION faction)
        : mFaction(faction)
    {};
    ~C_LawCard(){};

public:
    const E_FACTION &getFaction()const{ return mFaction; };
    void setFaction(const E_FACTION &type){ mFaction = type; };

protected:
    E_FACTION mFaction;
};

#endif // C_LAWCARD_H
