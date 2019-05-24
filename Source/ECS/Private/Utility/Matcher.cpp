#include "ECS.h"
#include "Matcher.h"
#include <algorithm>
#include "GroupEventType.h"
#include "TriggerOnEvent.h"

auto UMatcher::AllOf(const ComponentIdList indices) -> const UMatcher*
{
	UMatcher* matcher = NewObject<UMatcher>();
	matcher->mAllOfIndices = DistinctIndices(indices);
	matcher->CalculateHash();

	return matcher;
}

auto UMatcher::AllOf(const TArray<UMatcher*> Matchers) -> const UMatcher*
{
	return UMatcher::AllOf(MergeIndices(Matchers));
}

auto UMatcher::AnyOf(const ComponentIdList indices) -> const UMatcher*
{
	UMatcher* matcher = NewObject<UMatcher>();
	matcher->mAnyOfIndices = DistinctIndices(indices);
	matcher->CalculateHash();
	return matcher;
}

	auto UMatcher::AnyOf(const TArray<UMatcher*> Matchers) -> const UMatcher*
{
	return UMatcher::AnyOf(MergeIndices(Matchers));
}

auto UMatcher::NoneOf(const ComponentIdList indices) -> const UMatcher*
{
	UMatcher* matcher = NewObject<UMatcher>();
	matcher->mNoneOfIndices = DistinctIndices(indices);
	matcher->CalculateHash();

	return matcher;
}

auto UMatcher::NoneOf(const TArray<UMatcher*> Matchers) -> const UMatcher*
{
	return UMatcher::NoneOf(MergeIndices(Matchers));
}

	bool UMatcher::IsEmpty() const
{
	return (mAllOfIndices.empty() && mAnyOfIndices.empty() && mNoneOfIndices.empty());
}

bool UMatcher::Matches(const AEntity* entity)
{
	auto matchesAllOf = mAllOfIndices.empty() || entity->HasComponents(mAllOfIndices);
	auto matchesAnyOf = mAnyOfIndices.empty() || entity->HasAnyComponent(mAnyOfIndices);
	auto matchesNoneOf = mNoneOfIndices.empty() || !entity->HasAnyComponent(mNoneOfIndices);

	return matchesAllOf && matchesAnyOf && matchesNoneOf;
}

auto UMatcher::GetIndices() -> const ComponentIdList
{
	if (mIndices.empty())
	{
		mIndices = MergeIndices();
	}

return mIndices;
}

	auto UMatcher::GetAllOfIndices() const -> const ComponentIdList
{
	return mAllOfIndices;
}

	auto UMatcher::GetAnyOfIndices() const -> const ComponentIdList
{
	return mAnyOfIndices;
}

	auto UMatcher::GetNoneOfIndices() const -> const ComponentIdList
{
	return mNoneOfIndices;
}

	auto UMatcher::GetHashCode() const -> unsigned int
{
	return mCachedHash;
}

bool UMatcher::CompareIndices(const UMatcher* Matcher) const
{
	if (Matcher->IsEmpty())
	{
		return false;
	}

	auto leftIndices = this->MergeIndices();
	auto rightIndices = Matcher->MergeIndices();

	if (leftIndices.size() != rightIndices.size())
	{
		return false;
	}

	for (unsigned int i = 0, count = leftIndices.size(); i < count; ++i)
	{
		if (leftIndices[i] != rightIndices[i])
		{
			return false;
		}
	}

	return true;
}

auto UMatcher::OnEntityAdded() -> const FTriggerOnEvent
{
	FTriggerOnEvent data;
	data.Init(this, GroupEventType::OnEntityAdded);
	return data;
	//return FTriggerOnEvent().Init(this, GroupEventType::OnEntityAdded);
}

	auto UMatcher::OnEntityRemoved() -> const FTriggerOnEvent
{
	FTriggerOnEvent data;
	data.Init(this, GroupEventType::OnEntityRemoved);
	return data;
//	return FTriggerOnEvent().Init(this, GroupEventType::OnEntityRemoved);
}

	auto UMatcher::OnEntityAddedOrRemoved() -> const FTriggerOnEvent
{
	FTriggerOnEvent data;
	data.Init(this, GroupEventType::OnEntityAddedOrRemoved);
	return data;
//	return FTriggerOnEvent().Init(this, GroupEventType::OnEntityAddedOrRemoved);
}

	bool UMatcher::operator ==(const UMatcher* right) const
{
	return this->GetHashCode() == right->GetHashCode() && this->CompareIndices(right);
}

auto UMatcher::MergeIndices() const -> ComponentIdList
{
	auto indicesList = ComponentIdList();
	indicesList.reserve(mAllOfIndices.size() + mAnyOfIndices.size() + mNoneOfIndices.size());

	for (const auto &id : mAllOfIndices)
	{
		indicesList.push_back(id);
	}

	for (const auto &id : mAnyOfIndices)
	{
		indicesList.push_back(id);
	}

	for (const auto &id : mNoneOfIndices)
	{
		indicesList.push_back(id);
	}

	return DistinctIndices(indicesList);
}

void UMatcher::CalculateHash()
{
	unsigned int hash = typeid(UMatcher).hash_code();

	hash = ApplyHash(hash, mAllOfIndices, 3, 53);
	hash = ApplyHash(hash, mAnyOfIndices, 307, 367);
	hash = ApplyHash(hash, mNoneOfIndices, 647, 683);

	mCachedHash = hash;
}

auto UMatcher::ApplyHash(unsigned int hash, const ComponentIdList indices, int i1, int i2) const -> unsigned int
{
	if (indices.size() > 0)
	{
		for (int i = 0, indicesLength = indices.size(); i < indicesLength; i++)
		{
			hash ^= indices[i] * i1;
		}

		hash ^= indices.size() * i2;
	}

	return hash;
}

auto UMatcher::MergeIndices(TArray<UMatcher*> Matchers) -> ComponentIdList
{
	unsigned int totalIndices = 0;

	for (auto Matcher : Matchers)
	{
		totalIndices += Matcher->GetIndices().size();
	}

	auto indices = ComponentIdList();
	indices.reserve(totalIndices);

	for (auto Matcher : Matchers)
	{
		for (const auto &id : Matcher->GetIndices())
		{
			indices.push_back(id);
		}
	}

	return indices;
}

auto UMatcher::DistinctIndices(ComponentIdList indices) -> ComponentIdList
{
	std::sort(indices.begin(), indices.end());
	indices.erase(std::unique(indices.begin(), indices.end()), indices.end());

	return indices;

	// Old Code (delete!)
	/*auto indicesSet = unordered_set<unsigned int>(indices.begin(), indices.end());

	auto uniqueIndices = ComponentIdList();
	uniqueIndices.reserve(indicesSet.size());

	for(const auto &id : indicesSet)
	{
	uniqueIndices.push_back(id);
	}

	std::sort(uniqueIndices.begin(), uniqueIndices.end(), [](unsigned int a, unsigned int b) {
	return b < a;
	});

	return uniqueIndices;*/
}

