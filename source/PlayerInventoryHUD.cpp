#include "PlayerInventoryHUD.hpp"

#include "gui/GUIRenderer.hpp"
#include "Application.hpp"

namespace vw
{
	PlayerInventoryHUD::PlayerInventoryHUD ( Application & application, Inventory & inventory )
		:
		application ( &application ),
		inventory ( &inventory )
	{
		application.GetWindow ().AddScrollCallback ( [this] ( glm::vec2 const & delta ) {
			if ( ! this->application->CheckPaused () )
			{
				int newSelectedSlotIndex = selectedSlotIndex + glm::sign ( -delta.y );

				if ( newSelectedSlotIndex >= 0 && newSelectedSlotIndex <= 5 )
					selectedSlotIndex = newSelectedSlotIndex;
			}
		} );
	}

	void PlayerInventoryHUD::Update ( float deltaTime )
	{
	}

	void PlayerInventoryHUD::Render ( GUIRenderer & guiRenderer )
	{
		std::vector <Panel> panels;

		glm::vec2 windowSize { guiRenderer.GetWindowSize () };

		glm::vec2 slotCount { 6, 1 };
		float slotSize { 60 };
		float padding { 5.0f };
		float bottomMargin { 5.0f };

		glm::vec2 backgroundSize { slotSize * slotCount + ( ( slotCount + 1.0f ) * padding ) };

		glm::vec2 backgroundPosition {
			( windowSize.x - backgroundSize.x ) * 0.5,
			windowSize.y - backgroundSize.y - bottomMargin
		};

		panels.push_back ( { backgroundPosition, backgroundSize, { 0.0f, 0.0f, 0.0f, 1.0f } } );

		float stride { slotSize + padding };
		glm::vec2 slotPosition { backgroundPosition + padding };

		for ( int slotIndex { 0 }; slotIndex < slotCount.x; ++slotIndex )
		{
			if ( slotIndex == selectedSlotIndex )
			{
				glm::vec2 position { slotPosition - padding };
				auto size { glm::vec2 { slotSize, slotSize } + padding * 2.0f };

				panels.push_back ( { position, size, { 1.0f, 1.0f, 0.0f, 1.0f } } );
			}

			auto inventorySlot { inventory->GetSlot ( slotIndex ) };
			GLuint texture { 0 };

			if ( ! inventorySlot.CheckEmpty () )
				texture = application->GetItemTexture ( inventorySlot.GetItemStack ().itemType );

			panels.push_back ( { slotPosition, { slotSize, slotSize }, { 1.0f, 1.0f, 1.0f, 1.0f }, texture } );

			slotPosition.x += stride;
		}

		for ( auto const & panel : panels )
			guiRenderer.RenderPanel ( panel );
	}

	int PlayerInventoryHUD::GetSelectedSlotIndex () const
	{
		return selectedSlotIndex;
	}
}