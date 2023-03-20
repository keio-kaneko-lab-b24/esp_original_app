#ifndef CONSTANTS_H_
#define CONSTANTS_H_

// サンプリング周波数（1秒間に何回筋電位を取得するか）
constexpr int TARGET_HZ = 1000;
// 判定頻度（1秒間に何回判定するか）
constexpr int PREDICT_HZ = 10;
// 入力配列の時間（何秒間分を判定に使用するか）
const float NEEDS_TIME_SEC = 0.25;
// 入力配列の要素数（直近{RAW_LENGTH}個分の筋電を常に保持しておく）
const int RAW_LENGTH = (int)(TARGET_HZ * NEEDS_TIME_SEC);

// 移動平均に使用する窓枠サイズ
const int WINDOW_SIZE = 10;

#endif // CONSTANTS_H_