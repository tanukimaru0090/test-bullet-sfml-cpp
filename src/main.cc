#include <cmath>
#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
//
// メインプログラム
// 弾幕シューティングでの模範コード
//

using namespace sf;
//
// xy値を正規化してベクターにして返す
//
Vector2f normalizePosition(float x, float y) {
    auto length = std::sqrt(x * x + y * y);
    auto resX = x / length;
    auto resY = y / length;
    Vector2f v((float)resX, (float)resY);
    return v;
}
//
//  弾の角度を計算して浮動小数点で返す
//
float bulletAngle(Vector2f playerPos, Vector2f enemyPos) {
    auto dx = playerPos.x - enemyPos.x;
    auto dy = playerPos.y - enemyPos.y;
    auto normalPos = normalizePosition(dx, dy);
    auto angle = std::atan2(normalPos.y, normalPos.x);
    return angle;
}
//
// 弾とオブジェクト(プレイヤーなど)が衝突してるかどうかの判定
// 弾とオブジェクトの種類は通常はSpriteなど
// また、自機の当たり判定はつけない
//
bool isObjectCollision(RectangleShape obj1, RectangleShape obj2) {
    auto objBounds1 = obj1.getLocalBounds();
    auto objBounds2 = obj2.getLocalBounds();
    // obj1とobj2の変換を取得する
    auto objTransform1 = obj1.getTransform();
    auto objTransform2 = obj2.getTransform();
    // obj1とobj2の矩形を変換する
    auto objRect1 = objTransform1.transformRect(objBounds1);
    auto objRect2 = objTransform2.transformRect(objBounds2);
    // 変換した矩形で衝突判定を行う
    if (objRect1.intersects(objRect2)) {
        return true;
    }
    return false;
}
int main(int argv, char** argc) {
    const int W_WIDITH = 640;   // ウィンドウの横幅
    const int W_HEIGHT = 480;   // ウィンドウの縦幅
    const int W_BITCOLOR = 32;  // ウィンドウのカラービット

    //
    // 敵やプレイヤーのサイズの定義
    //
    Vector2f bulletSize(15, 15);
    Vector2f playerSize(50, 50);
    Vector2f enemySize(50, 50);

    //
    // 敵やプレイヤーの初期位置の定義
    //
    Vector2f enemyPos(W_WIDITH / 2, 50);
    Vector2f playerPos(W_WIDITH / 2, W_HEIGHT - 50);
    Vector2f bulletPos(enemyPos.x + 10, enemyPos.y + 10);
    //
    // ウィンドウを作成
    // 
    
    RenderWindow window(VideoMode(W_WIDITH, W_HEIGHT, W_BITCOLOR), "てすとー");

    // auto music = new
    // Music("/home/daruma/ダウンロード/th-music/麓の神社.wav"); music.Play();

    //
    // 敵やプレイヤーを作成
    //
    RectangleShape enemyShape(enemySize);
    RectangleShape bulletShape(bulletSize);
    RectangleShape playerShape(playerSize);
    //
    // 弾の速度 (Default:1)
    //
    auto bulletSpeed = 0.5;
    //
    // 敵やプレイヤーの色や位置を設定
    //
    bulletShape.setFillColor(Color::Black);
    playerShape.setFillColor(Color::Blue);
    enemyShape.setFillColor(Color::Red);
    bulletShape.setPosition(bulletPos);
    playerShape.setPosition(playerPos);
    enemyShape.setPosition(enemyPos);

    // 敵やプレイヤーの原点を中心にそれぞれ設定
    bulletShape.setOrigin(Vector2f(bulletShape.getLocalBounds().width / 2,
                                      enemyShape.getLocalBounds().height / 2));

    enemyShape.setOrigin(Vector2f(enemyShape.getLocalBounds().width / 2,
                                     enemyShape.getLocalBounds().height / 2));
    playerShape.setOrigin(Vector2f(playerShape.getLocalBounds().width / 2,
                                      playerShape.getLocalBounds().height / 2));

    while (window.isOpen()) {
        Event event;
        while(window.pollEvent(event)){
            if(event.type == Event::Closed){
                window.close();
            }
        }
        //
        // 位置をセット
        // これをすることで座標のベクタ経由で位置を変えることができる
        //
        bulletShape.setPosition(bulletPos);
        playerShape.setPosition(playerPos);
        enemyShape.setPosition(enemyPos);
        //
        // 更新
        //

        //
        // プレイヤーのキー処理
        //

        if (Keyboard::isKeyPressed(Keyboard::Key::Right)) {
            playerPos.x += 5;
        }
        if (Keyboard::isKeyPressed(Keyboard::Key::Left)) {
            playerPos.x -= 5;
        }

        if (Keyboard::isKeyPressed(Keyboard::Key::Up)) {
            playerPos.y -= 5;
        }

        if (Keyboard::isKeyPressed(Keyboard::Key::Down)) {
            playerPos.y += 5;
        }
        if (isObjectCollision(bulletShape, playerShape)) {
            std::cout << "弾がプレイヤーにあたったよーー" << std::endl;
        }
        //
        // プレイヤーと敵との角度を得る
        //
        auto theta = bulletAngle(playerPos, enemyPos);
        //
        // 弾のスピードを計算し、弾のベクトルに加算する
        //
        auto vx = bulletSpeed * std::cos(theta);
        auto vy = bulletSpeed * std::sin(theta);
        bulletPos.x += (float)vx;
        bulletPos.y += (float)vy;

        //
        // イベント処理
        //
        window.clear(Color::White);

        //
        // 描画
        //
        window.draw(enemyShape);
        window.draw(playerShape);
        window.draw(bulletShape);

        window.display();
    }
}

