#pragma once
#include "Entity.h"
#include "Matcher.generated.h"

//匹配筛选器

struct FTriggerOnEvent;

UCLASS()
class ECS_API UMatcher : public UObject
{
	GENERATED_BODY()
public:
	static auto AllOf(const ComponentIdList indices) -> const UMatcher*;
	static auto AllOf(const TArray<UMatcher*> Matchers) -> const UMatcher*;
	static auto AnyOf(const ComponentIdList indices) -> const UMatcher*;
	static auto AnyOf(const TArray<UMatcher*> Matchers) -> const UMatcher*;
	static auto NoneOf(const ComponentIdList indices) -> const UMatcher*;
	static auto NoneOf(const TArray<UMatcher*> Matchers) -> const UMatcher*;

	bool IsEmpty() const;
	bool Matches(const AEntity* entity);
	auto GetIndices() -> const ComponentIdList;
	auto GetAllOfIndices() const -> const ComponentIdList;
	auto GetAnyOfIndices() const -> const ComponentIdList;
	auto GetNoneOfIndices() const -> const ComponentIdList;

	auto GetHashCode() const -> unsigned int;
	bool CompareIndices(const UMatcher* Matcher) const;

	auto OnEntityAdded() -> const FTriggerOnEvent;
	auto OnEntityRemoved() -> const FTriggerOnEvent;
	auto OnEntityAddedOrRemoved() -> const FTriggerOnEvent;

	bool operator ==(const UMatcher* right) const;

protected:
	void CalculateHash();

	ComponentIdList mIndices;
	ComponentIdList mAllOfIndices;
	ComponentIdList mAnyOfIndices;
	ComponentIdList mNoneOfIndices;

private:
	auto ApplyHash(unsigned int hash, const ComponentIdList indices, int i1, int i2) const -> unsigned int;
	auto MergeIndices() const->ComponentIdList;
	static auto MergeIndices(TArray<UMatcher*> UMatchers)->ComponentIdList;
	static auto DistinctIndices(ComponentIdList indices)->ComponentIdList;

	unsigned int mCachedHash{ 0 };
};


namespace std
{
	template <>
	struct hash<UMatcher>
	{
		std::size_t operator()(const UMatcher& UMatcher) const
		{
			return hash<unsigned int>()(UMatcher.GetHashCode());
		}
	};
}

namespace
{
#define FUNC_1(MODIFIER, X) MODIFIER(X)
#define FUNC_2(MODIFIER, X, ...) MODIFIER(X), FUNC_1(MODIFIER, __VA_ARGS__)
#define FUNC_3(MODIFIER, X, ...) MODIFIER(X), FUNC_2(MODIFIER, __VA_ARGS__)
#define FUNC_4(MODIFIER, X, ...) MODIFIER(X), FUNC_3(MODIFIER, __VA_ARGS__)
#define FUNC_5(MODIFIER, X, ...) MODIFIER(X), FUNC_4(MODIFIER, __VA_ARGS__)
#define FUNC_6(MODIFIER, X, ...) MODIFIER(X), FUNC_5(MODIFIER, __VA_ARGS__)
#define GET_MACRO(_1, _2, _3, _4, _5, _6, NAME,...) NAME
#define FOR_EACH(MODIFIER,...) GET_MACRO(__VA_ARGS__, FUNC_6, FUNC_5, FUNC_4, FUNC_3, FUNC_2, FUNC_1)(MODIFIER, __VA_ARGS__)

#define COMPONENT_GET_TYPE_ID(COMPONENT_CLASS) FComponentTypeId::Get<COMPONENT_CLASS>()
#define Matcher_AllOf(...) (UMatcher*)UMatcher::AllOf(std::vector<ComponentId>({ FOR_EACH(COMPONENT_GET_TYPE_ID, __VA_ARGS__) }))
#define Matcher_AnyOf(...) (UMatcher*)UMatcher::AnyOf(std::vector<ComponentId>({ FOR_EACH(COMPONENT_GET_TYPE_ID, __VA_ARGS__) }))
#define Matcher_NoneOf(...) (UMatcher*)UMatcher::NoneOf(std::vector<ComponentId>({ FOR_EACH(COMPONENT_GET_TYPE_ID, __VA_ARGS__) }))
}
