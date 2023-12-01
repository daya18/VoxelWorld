#include "InventoryView.hpp"

#include "gui/GUIRenderer.hpp"
#include "Application.hpp"

namespace vw
{
	InventoryView::InventoryView ( Application & application, Inventory & inventory )
	: 
		application ( &application ),
		inventory ( &inventory )
	{
	}

	void InventoryView::Render ( GUIRenderer & guiRenderer )
	{
		Panel background;
		std::vector <Panel> slots;

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

		background = { backgroundPosition, backgroundSize, { 0.0f, 0.0f, 0.0f, 1.0f } };

		float stride { slotSize + padding };
		glm::vec2 slotPosition { backgroundPosition + padding };

		int slotIndex { 0 };
		for ( int slotIndexY { 0 }; slotIndexY < slotCount.y; ++slotIndexY )
		{
			for ( int slotIndexX { 0 }; slotIndexX < slotCount.x; ++slotIndexX )
			{
				auto inventorySlot { inventory->GetSlot ( slotIndex ) };
				GLuint texture { 0 };

				if ( ! inventorySlot.CheckEmpty () )
					texture = application->GetItemTexture ( inventorySlot.GetItemStack().itemType);

				slots.push_back ( { slotPosition, { slotSize, slotSize }, { 1.0f, 1.0f, 1.0f, 1.0f }, texture } );

				slotPosition.x += stride;
				++slotIndex;
			}

			slotPosition.x = backgroundPosition.x + padding;
			slotPosition.y += stride;
		}

		guiRenderer.RenderPanel ( background );

		for ( auto const & slot : slots )
			guiRenderer.RenderPanel ( slot );
	}
}