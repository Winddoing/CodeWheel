#ifndef WAVEDATA_H
#define WAVEDATA_H

int insertWaveFile(const char *fileName);
//可以按文件名查找，如果fileName为NULL时
//代表查找所有数据
int selectWaveFile(const char *fileName);
int deleteWaveFile(const char *id);
int playWaveFile(const char *id);

#endif // WAVEDATA_H

