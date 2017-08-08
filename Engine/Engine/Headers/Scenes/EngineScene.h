#pragma once
#include "Utils\ArrayList.h"
#include "GameObjects\GameObject.h"
#include "Renderer\Renderer.h"
#include "Managers\CacheManager.h"
#include "TiledImporter\TiledImporter.h"
#include "Component\SpriteRenderer.h"

class EngineScene {

public:
	EngineScene(XEngine& ref):coreRef(ref)
	{
	}

	~EngineScene() {
		renderList.empty();
		updateList.empty();
	}

	void Preload() {}

	virtual void Start() = 0;
	virtual void Update(float deltaTime) {
		for (int i = updateList.size - 1 ; i >= 0; i--) {
			GameObject& go = *updateList[i];
			if (go.isPendingDestroy) {
				delete updateList[i];
				updateList.erase(i);
			}
			else {
				go.Update(deltaTime);
			}
			
		}

	}
	virtual void OnDestroy() = 0;
	void Render(Renderer &renderer) {
		renderer.OnRenderGroup(renderList);
	}
protected:
	void AddGameObject(GameObject* go, bool mustUpdate = true, bool mustRender = true) {
		
		if (mustUpdate) {
			updateList.insert(go);
		}

		if (mustRender) {
			renderList.insert(go);
		}
	}

	void AddTiledMap(char* source, GameObject* objectBetweenLayers = nullptr, int overLayerIndex = 0) {
		tmx::Map *map = new tmx::Map();
		tmx::TiledImporter::LoadMap(source, map);
		vector<CachedImage*> cachedImages;
		string resourceFolder = "Resources/";
		int tilesetCount = 0;
		for (int i = 0; i < 10; i++) {
			if (map->tileSets[i].image.source != NULL && map->tileSets[i].image.source != L"") {
				std::wstring output;

				output = std::wstring(resourceFolder.begin(), resourceFolder.end());
				output = output + std::wstring(map->tileSets[i].image.source); // or output += L" program";
				const wchar_t *ptr = output.c_str();

				cachedImages.push_back(CacheManager::GetInstance()->AddImage(ptr));
				tilesetCount++;
			}
		}
		for (int i = 0; i < 20; i++) {
			if (map->layers[i].data.encoding != NULL && map->layers[i].data.encoding != "") {
				tmx::Data data = map->layers[i].data;
				if (objectBetweenLayers != nullptr && i == overLayerIndex) {
					AddGameObject(objectBetweenLayers);
				}
				for (int j = 0; j < data.value.capacity(); j++) {
					vector<string> columns = data.value.at(j);
					for (int k = 0; k < map->width; k++) {
						int index = std::stoi(columns.at(k));
						if (index == 0) continue;

						int tileSetIndex = GetTileSetIndex(map->tileSets, index, tilesetCount);
						tmx::Tileset* currTileset = &map->tileSets[tileSetIndex];
						Vector2 spawnPos;

						spawnPos.x = (float) map->tilewidth * k;
						spawnPos.y = (float) (map->tileheight * map->height - map->tileheight) -map->tileheight *j;

						GameObject* go = new GameObject(spawnPos, coreRef);
						SpriteRenderer* spriteRenderer = go->AddComponent<SpriteRenderer>(false, true);
						spriteRenderer->SetImage(cachedImages.at(tileSetIndex));
						spriteRenderer->SetSpriteSheet(currTileset->tilewidth, currTileset->tileheight);
						spriteRenderer->currentFrame = index - currTileset->firstgid;

						AddGameObject(go, false, true);
					}
				}
			}
		}

		for (int i = 0; i < 10; i++) {
			if (map->objectGroups[i].name != NULL && map->objectGroups[i].name != "") {
				vector<tmx::Object> objects = map->objectGroups[i].objects;

				for (int j = 0; j < objects.capacity(); j++) {
					tmx::Object object = objects[j];

					Vector2 spawnPos;

					spawnPos.x = (float)object.x + object.width / 2.f;
					spawnPos.y = ((float)(map->tileheight * map->height) - (object.y + object.height / 2.f));

					coreRef.physics->CreateBoxBody(spawnPos, Vector2(object.width / 2.f, object.height / 2.f), 0.0f, 1.0f, PhysicBodyType::Static, false);
				}
			}
		}
	}

private:
	int GetTileSetIndex(tmx::Tileset tilesets[], int tileIndex, int size) {
		for (int i = 0; i < size - 1; i++) {
			int firstGid = tilesets[i].firstgid;
			int nextFirstGid = tilesets[i + 1].firstgid;
			if (tileIndex >= firstGid && tileIndex < nextFirstGid) {
				return i;
			}
		}
		return size - 1;
	}

public:
	bool pendingActivation = true;
	ArrayList<GameObject*> renderList;
	ArrayList<GameObject*> updateList;

protected:
	XEngine& coreRef;
};