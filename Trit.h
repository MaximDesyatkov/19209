#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>
#include <cmath>

enum class trit
{
	False = 0,
	Unknown = 1,
	True = 3
};

trit operator!(trit a);
trit operator|(trit a, trit b);
trit operator&(trit a, trit b);
std::ostream& operator<<(std::ostream& os, trit a);

using namespace std;
typedef unsigned int uint;
typedef unsigned long long ull;

static uint unknownMask = 0;

class TritSet
{
private:
	vector<uint> arr;
	ull count = 0;

	typedef struct EnumClassHash
	{
		template <typename T>
		size_t operator()(T t) const
		{
			return static_cast<size_t>(t);
		}
	} tritHash;

	void setValue(ull index0, uint index1, trit value);
	void setValue(ull index, trit value);
	trit getValue(ull index) const;
	trit getValue(ull index0, uint index1) const;
public:
	TritSet(ull capacity);
	TritSet(TritSet&& a) noexcept;
	

	ull capacity();
	ull tritSize();
	void shrink();
	void trim(ull lastIndex);
	ull length();
	ull cardinality(trit value);
	unordered_map<trit, int, tritHash> cardinality();


	typedef struct trit_t
	{
	private:
		TritSet& set;
		ull nValue;
	public:
		trit_t(TritSet &other, size_t index) : set(other)
		{
			nValue = index;
		}
		trit_t(ull r, TritSet& s) : nValue(r), set(s) {}
		~trit_t() = default;
		trit_t& operator *()
		{
			return *this;
		}

		bool operator !=(size_t index)
		{
			if (this->nValue != index)
			{
				return true;
			}
			return false;
		}
		
		void operator ++()
		{
			this->nValue++;
		}

		operator trit() const
		{
			if (nValue >= set.count)
				return trit::Unknown;
			else
				return set.getValue(nValue);
		}

		const TritSet& getSet()
		{
			return set;
		}

		ull getIndex()
		{
			return nValue;
		}

		trit operator|(const trit& t) const
		{
			if (nValue >= set.count)
				return trit::Unknown;
			else
				return t | set.getValue(nValue);
		}

		trit operator|(const trit_t& t) const
		{
			if (nValue >= set.count)
				return trit::Unknown;
			else
				return t | set.getValue(nValue);
		}

		trit operator&(const trit& t) const
		{
			if (nValue >= set.count)
				return trit::Unknown;
			else
				return t & set.getValue(nValue);
		}

		trit operator&(const trit_t& t) const
		{
			if (nValue >= set.count)
				return trit::Unknown;
			else
				return t & set.getValue(nValue);
		}

		trit operator!() const
		{
			if (nValue >= set.count)
				return trit::Unknown;
			else
				return !set.getValue(nValue);
		}

		bool operator==(const trit& t) const
		{
			if (nValue >= set.count)
				return trit::Unknown == t;
			else
				return t == set.getValue(nValue);
		}

		bool operator==(const trit_t& t) const
		{
			if (nValue >= set.count)
				return false;
			else
				return t == set.getValue(nValue);
		}

		bool operator!=(const trit& t) const
		{
			if (nValue >= set.count)
				return false;
			else
				return t != set.getValue(nValue);
		}

		bool operator!=(const trit_t& t) const
		{
			if (nValue >= set.count)
				return false;
			else
				return t != set.getValue(nValue);
		}

		trit_t& operator=(const trit& t)
		{
			if (!unknownMask)
				for (size_t i = 0; i < 8 * sizeof(uint); ++i)
					unknownMask |= i % 2 == 0 ? 1u << i : 0;

			if (t != trit::Unknown && nValue >= set.count)
			{
				set.arr.resize(ceil((2.0 * (1.0 + nValue)) / (8.0 * sizeof(uint))), unknownMask);
				set.count = nValue + 1;
				set.setValue(nValue, t);
			}
			else if (nValue < set.count)
				set.setValue(nValue, t);

			return *this;
		}
	} tritReference;

	trit_t begin()
	{
		trit_t tr(*this, 0);
		return tr;
	}

	size_t TritSet::end()
	{
		return this->count;
	}

	friend ostream& operator<< (ostream& stream, TritSet::tritReference t)
	{
		if (t.getIndex() >= t.getSet().count)
			return stream << trit::Unknown;
		else
			return stream << t.getSet().getValue(t.getIndex());
	}

	tritReference operator[](ull b);
	TritSet operator&(TritSet& a);
	TritSet operator|(TritSet& a);
	TritSet operator!();
};