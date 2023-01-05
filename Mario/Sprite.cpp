#include "Sprite.h"

Sprite::Sprite(int id, int left, int top, int right, int bottom, LPTEXTURE tex)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	this->texture = tex;

	float texWidth = (float)tex->getWidth();
	float texHeight = (float)tex->getHeight();

	// Set the sprite’s shader resource view
	sprite.pTexture = tex->getShaderResourceView();

	sprite.TexCoord.x = this->left / texWidth;
	sprite.TexCoord.y = this->top / texHeight;

	int spriteWidth = (this->right - this->left );
	int spriteHeight = (this->bottom - this->top );

	sprite.TexSize.x = spriteWidth / texWidth;
	sprite.TexSize.y = spriteHeight / texHeight;

	sprite.ColorModulate = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	sprite.TextureIndex = 0;

	D3DXMatrixScaling(&this->matScaling, (FLOAT)spriteWidth, (FLOAT)spriteHeight, 1.0f);
}

void Sprite::Draw(int nx, float x, float y, int alpha)
{
	//sprite.ColorModulate = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha / 255.0f);

	Game* game = Game::GetInstance();
	//game->Draw(nx, x, y, texture, left, top, right, bottom, alpha);
	
	float cx, cy;
	game->GetCamPos(cx, cy);

	cx = (FLOAT)floor(cx);
	cy = (FLOAT)floor(cy);

	D3DXMATRIX matTranslation;
	
	x = (FLOAT)floor(x);
	y = (FLOAT)floor(y);

	D3DXMatrixTranslation(&matTranslation, x - cx, game->GetScreenHeight() - y + cy, 0.2f);
	
	this->sprite.matWorld = (this->matScaling * matTranslation );

	game->GetSpriteHandler()->DrawSpritesImmediate(&sprite, 1, 0, 0);
	return;
	//--------------------------------------------------

	// calculate position of object in real world
	D3DXVECTOR3 p(x - cx, game->GetScreenHeight() - y + cy, 0.2f);

	// flip sprite, using nx parameter
	D3DXMATRIX oldTransform;
	D3DXMATRIX newTransform;

	game->GetSpriteHandler()->GetViewTransform(&oldTransform);

	D3DXVECTOR2 center = D3DXVECTOR2(p.x + (right - left) / 2, p.y + (bottom - top) / 2);
	D3DXVECTOR2 rotate = D3DXVECTOR2(nx > 0 ? -1 : 1, 1);

	// Xây dựng một ma trận 2D lưu thông tin biến đổi (scale, rotate)
	D3DXMatrixTransformation2D(&newTransform, &center, 0.0f, &rotate, NULL, 0.0f, NULL);

	// Cần nhân với ma trận cũ để tính ma trận biến đổi cuối cùng
	D3DXMATRIX finalTransform = newTransform * oldTransform;

	game->GetSpriteHandler()->SetViewTransform(&finalTransform);

	game->GetSpriteHandler()->DrawSpritesImmediate(&sprite, 1, 0, 0);

	game->GetSpriteHandler()->SetViewTransform(&oldTransform);

}

