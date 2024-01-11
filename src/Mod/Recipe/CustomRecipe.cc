#include "Global.h"
#include "GMLIB/Mod/CustomRecipe.h"

using RecipesMap = std::map<
    class HashedString,
    class std::map<
        std::string,
        class std::shared_ptr<class Recipe>,
        struct std::less<std::string>,
        class std::allocator<struct std::pair<std::string const, class std::shared_ptr<class Recipe>>>>,
    struct std::less<class HashedString>,
    class std::allocator<struct std::pair<
        class HashedString const,
        class std::map<
            std::string,
            class std::shared_ptr<class Recipe>,
            struct std::less<std::string>,
            class std::allocator<struct std::pair<std::string const, class std::shared_ptr<class Recipe>>>>>>>;



template <class T>
    requires std::is_base_of<GMLIB_CustomShapedRecipe, T>::value
void GMLIB_CustomRecipe::registerShapedRecipe() {
    SharedPtr<T> ShapedRecipe = SharedPtr<T>::makeShared();
    return ll::service::bedrock::getLevel()->getRecipes().addShapedRecipe(
        ShapedRecipe->getRecipeId(),
        ShapedRecipe->getResult(),
        ShapedRecipe->getShape(),
        ShapedRecipe->getIngredients(),
        ShapedRecipe->getCraftingTags(),
        ShapedRecipe->getPriority(),
        ShapedRecipe->getConstructor(),
        ShapedRecipe->getRecipeUnlockingRequirement(),
        ShapedRecipe->getSemVersion()
    );
}

template <class T>
    requires std::is_base_of<GMLIB_CustomShapelessRecipe, T>::value
void GMLIB_CustomRecipe::registerShapelessRecipe() {
    SharedPtr<T> ShapelessRecipe = SharedPtr<T>::makeShared();
    return ll::service::bedrock::getLevel()->getRecipes().addShapelessRecipe(
        ShapelessRecipe->getRecipeId(),
        ShapelessRecipe->getResult(),
        ShapelessRecipe->getIngredients(),
        ShapelessRecipe->getCraftingTags(),
        ShapelessRecipe->getPriority(),
        ShapelessRecipe->getConstructor(),
        ShapelessRecipe->getRecipeUnlockingRequirement(),
        ShapelessRecipe->getSemVersion()
    );
}

template <class T>
    requires std::is_base_of<GMLIB_CustomFurnaceRecipe, T>::value
void GMLIB_CustomRecipe::registerFurnaceRecipe() {
    SharedPtr<T> FurnaceRecipe = SharedPtr<T>::makeShared();
    return ll::service::bedrock::getLevel()->getRecipes().addFurnaceRecipeAuxData(
        FurnaceRecipe->getInput(),
        FurnaceRecipe->getResult(),
        FurnaceRecipe->getCraftingTags()
    );
}

template <class T>
    requires std::is_base_of<GMLIB_CustomShulkerBoxRecipe, T>::value
void GMLIB_CustomRecipe::registerShulkerBoxRecipe() {
    SharedPtr<T> ShulkerBoxRecipe = SharedPtr<T>::makeShared();
    return ll::service::bedrock::getLevel()->getRecipes().addShulkerBoxRecipe(
        ShulkerBoxRecipe->getRecipeId(),
        ShulkerBoxRecipe->getResult(),
        ShulkerBoxRecipe->getIngredients(),
        ShulkerBoxRecipe->getCraftingTags(),
        ShulkerBoxRecipe->getRecipeUnlockingRequirement(),
        ShulkerBoxRecipe->getSemVersion()
    );
}

bool GMLIB_CustomRecipe::unregisterRecipe(std::string recipe_id) {
    auto AllRecipes = ll::service::bedrock::getLevel()->getRecipes().getRecipesAllTags();
    for (auto& recipe : AllRecipes) {
        if (recipe.second.count(recipe_id)) {
            recipe.second.erase(recipe_id);
            CraftingDataPacket::prepareFromRecipes(ll::service::bedrock::getLevel()->getRecipes(), true)
                ->sendToClients();
            return true;
        }
    }
    return false;
}