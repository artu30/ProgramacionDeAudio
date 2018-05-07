#include <vec2.h>
#include <vector>
#include <litegfx.h>
#include <glfw3.h>
#include <cstdint>
#include <textureManager.h>
#include <ColliderFunctions.h>
#include <Collider.h>
#include <Sprite.h>
#include <World.h>

float World::getClearRed   () const { return mClearRed;   }
float World::getClearGreen () const { return mClearGreen; }
float World::getClearBlue  () const { return mClearBlue;  }

const ltex_t* World::getBackground(size_t layer) const {
	if (layer >= 0 && layer < mBackgrounds.size()) {
		return mBackgrounds.at(layer).mBackgroundTexture;
	}
	else {
		return nullptr;
	}
}

float World::getScrollRatio(size_t layer) const {
	if (layer >= 0 && layer < mBackgrounds.size()) {
		return mBackgrounds.at(layer).mScrollRatio;
	}
	else {
		return 0.0f;
	}
}
void World::setScrollRatio(size_t layer, float ratio) {
	if (layer >= 0 && layer < mBackgrounds.size()) {
		mBackgrounds.at(layer).mScrollRatio = ratio;
	}
}

const Vec2& World::getScrollSpeed(size_t layer) const {
	if (layer >= 0 && layer < mBackgrounds.size()) {
		return mBackgrounds.at(layer).mScrollSpeed;
	}
	else {
		return Vec2();
	}
}

void World::setScrollSpeed(size_t layer, const Vec2& speed) {
	if (layer >= 0 && layer < mBackgrounds.size()) {
		mBackgrounds.at(layer).mScrollSpeed.setX(speed.getX());
		mBackgrounds.at(layer).mScrollSpeed.setY(speed.getY());
	}
}

const Vec2& World::getCameraPosition() const {
	return mCameraPosition;
}

void World::setCameraPosition(const Vec2& pos) {
	mCameraPosition.setX(pos.getX());
	mCameraPosition.setY(pos.getY());
}

void World::addSprite(Sprite* sprite) {
	mSprites.push_back(sprite);
}

void World::removeSprite(Sprite& sprite) {
	for (auto it = mSprites.begin(); it < mSprites.end(); it++) {
		if (sprite.getTexture() == (*it)->getTexture()) {
			it = mSprites.erase(it);
		}
	}
}

void World::update(float deltaTime) {
	// Llamamos callback
	if (mSprites.at(0)->getCallback()) {
		Sprite::CallbackFunc func = mSprites.at(0)->getCallback();

		func(*mSprites.at(0), deltaTime);
	}

	int count = 0;
	for (auto it = mBackgrounds.begin(); it != mBackgrounds.end(); it++) {
		it->mBackgroundPosition.setX(it->mBackgroundPosition.getX() + (getScrollSpeed(count).getX() * deltaTime));
		it->mBackgroundPosition.setY(it->mBackgroundPosition.getY() + (getScrollSpeed(count).getY() * deltaTime));

		count++;
	}

	for (auto it = mSprites.begin(); it < mSprites.end(); it++) {
		(*it)->update(deltaTime);
	}
}

void World::draw(const Vec2& screenSize) {
	lgfx_clearcolorbuffer(mClearRed, mClearGreen, mClearBlue);

	Vec2 newCameraPos;
	newCameraPos.setX(clamp(mCameraPosition.getX(), mWorldSize.getX() - screenSize.getX(), 0.0f));
	newCameraPos.setY(clamp(mCameraPosition.getY(), mWorldSize.getY() - screenSize.getY(), 0.0f));

	setCameraPosition(newCameraPos);

	lgfx_setorigin(mCameraPosition.getX(), mCameraPosition.getY());

	int count = 0;
	for (auto it = mBackgrounds.begin(); it != mBackgrounds.end(); it++) {
		float u1 = 0.0f;
		float v1 = 0.0f;
		float u2 = 0.0f;
		float v2 = 0.0f;

		/* En este punto resto a 1 el ratio de cada mapa por una premisa. Si la camara se mueve a una velocidad
		y el mapa (por ejemplo el de nivel) se mueve a la misma velocidad a la derecha, nunca se verá su incremento visual, 
		sin embargo, si este fondo en concreto no se mueve (ya que es el nivel) la camara delimita que porción vamos a ver.
		Del mismo modo con el resto de fondos, si el segundo se mueve un 0.2 más lento (1 - 0.8) que la cámara se moverá más lento que el 
		primer nivel y más rápido que los sucesivos, y así con los demás fondos */
		float posX = mCameraPosition.getX() * (1 - getScrollRatio(count));
		float posY = mCameraPosition.getY() * (1 - getScrollRatio(count));

		u1 = -mBackgrounds.at(count).mBackgroundPosition.getX() / static_cast<float>(getBackground(count)->width);
		v1 = -mBackgrounds.at(count).mBackgroundPosition.getY() / static_cast<float>(getBackground(count)->height);

		u2 = u1 + mWorldSize.getX() / static_cast<float>(getBackground(count)->width);
		v2 = v1 + mWorldSize.getY() / static_cast<float>(getBackground(count)->height);

		ltex_drawrotsized(getBackground(count), posX, posY, 0.0f, 0.0f, 0.0f, mWorldSize.getX(), mWorldSize.getY(), u1, v1, u2, v2);
		
		count++;
	}
	
	for (auto it = mSprites.begin(); it < mSprites.end(); it++) {
		(*it)->draw();
	}
}

float World::clamp(float x, float u, float l) {
	return min(u, max(x, l));
}