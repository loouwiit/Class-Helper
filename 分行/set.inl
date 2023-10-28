inline Set::RandRisist operator&(Set::RandRisist left, Set::RandRisist right)
{
	return (Set::RandRisist)((unsigned char)left & (unsigned char(right)));
}

inline Set::RandRisist operator|(Set::RandRisist left, Set::RandRisist right)
{
	return (Set::RandRisist)((unsigned char)left | (unsigned char(right)));
}

inline bool operator&&(Set::RandRisist left, Set::RandRisist right)
{
	return (bool)((unsigned char)left & (unsigned char(right)));
}
