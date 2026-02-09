# uClibc-ng 優化指南 - 減少 RAM 使用量

## 目標
通過重新編譯 uClibc-ng，將 Toybox 的 RAM 使用量從目前的 **77.12 KB 降低到 64 KB 以下**

## 當前 uClibc RAM 消耗分析

### 可優化項目（總計約 ~60 KB）

| 組件 | 當前大小 | 可節省 | 配置選項 | 影響 |
|------|----------|--------|----------|------|
| **locale_mmap** | 29.14 KB | ~28 KB | 禁用 LOCALE | 無多語言支持 |
| **__pthread_handles** | 16.00 KB | ~16 KB | 禁用 THREADS | 無多執行緒 |
| **pthread_keys** | 8.00 KB | ~8 KB | 禁用 THREADS | 無多執行緒 |
| **_fixed_buffers** | 8.00 KB | ~4-6 KB | 減小 STDIO 緩衝 | I/O 性能下降 |
| **__global_locale_data** | 2.79 KB | ~2.5 KB | 禁用 LOCALE | 無多語言支持 |
| **_string_syserrmsgs** | 2.84 KB | ~1-2 KB | 簡化錯誤訊息 | 錯誤訊息簡化 |
| **__C_ctype_tables** | 2.25 KB | ~1 KB | 使用小型 ctype | 部分字符處理限制 |
| **其他小項** | ~0.5 KB | ~0.5 KB | 各種優化 | 微小影響 |
| **總計** | **69.26 KB** | **~60 KB** | | |

### 理論最小值
- **當前**: 77.12 KB (uClibc 69.26 KB + Toybox 7.86 KB)
- **優化後**: **17-20 KB** (uClibc 9-12 KB + Toybox 7.86 KB)
- **節省**: **~57-60 KB** (可達成 **< 64 KB 目標**，甚至可能達到 32 KB！)

## uClibc-ng 配置選項詳解

### 1. 禁用 Locale 支持（節省 ~30 KB）

**配置選項**:
```makefile
# 在 uClibc-ng .config 中
# CONFIG_UCLIBC_HAS_LOCALE is not set
# CONFIG_UCLIBC_HAS_XLOCALE is not set
# CONFIG_UCLIBC_HAS_GLIBC_DIGIT_GROUPING is not set
```

**影響**:
- ✗ 無法處理多語言字符集（UTF-8, Big5 等）
- ✗ `setlocale()`, `localeconv()` 等函數無效
- ✗ 無區域相關的數字、日期格式
- ✓ Toybox 的基本命令仍可正常運作（僅支援 ASCII/C locale）

**Toybox 相容性**: ⚠️ 部分命令可能受影響
- `ls` - 檔名排序可能不符合本地習慣
- `sort` - 排序結果僅按 ASCII
- 其他基本命令無影響

### 2. 禁用 Thread 支持（節省 ~24 KB）

**配置選項**:
```makefile
# CONFIG_UCLIBC_HAS_THREADS is not set
# CONFIG_UCLIBC_HAS_THREADS_NATIVE is not set
# CONFIG_PTHREADS_DEBUG_SUPPORT is not set
```

**影響**:
- ✗ 無法使用 pthread 函數
- ✗ 無多執行緒支持
- ✓ Toybox 不需要多執行緒（主要使用 fork）

**Toybox 相容性**: ✓ 完全相容
- Toybox 的命令都是單執行緒的
- 使用 `fork()` / `vfork()` 而非 `pthread`

### 3. 減小 STDIO 緩衝（節省 ~4-6 KB）

**配置選項**:
```makefile
UCLIBC_HAS_STDIO_BUFSIZ_256=y       # 預設可能是 4096
# UCLIBC_HAS_STDIO_BUFSIZ_4096 is not set
UCLIBC_HAS_STDIO_BUILTIN_BUFFER_SIZE=256
```

**影響**:
- ✗ 檔案 I/O 性能下降（頻繁系統調用）
- ✓ 對小型檔案操作影響不大

**Toybox 相容性**: ✓ 完全相容
- Toybox 自己有 toybuf/libbuf 緩衝
- 減小 libc stdio 緩衝不影響功能

### 4. 精簡 Error Messages（節省 ~1-2 KB）

**配置選項**:
```makefile
# CONFIG_UCLIBC_HAS_SYS_ERRLIST is not set
UCLIBC_HAS_ERRNO_MESSAGES=y
# CONFIG_UCLIBC_HAS_SYS_SIGLIST is not set
```

**影響**:
- ✗ `strerror()` 返回簡化訊息
- ✗ 錯誤除錯資訊減少
- ✓ 基本錯誤處理仍可用

### 5. 禁用不需要的功能（節省 ~5-10 KB）

**配置選項**:
```makefile
# 網路功能（如果不需要）
# CONFIG_UCLIBC_HAS_IPV6 is not set
# CONFIG_UCLIBC_HAS_RPC is not set

# 正則表達式（如果不用 grep/sed）
# CONFIG_UCLIBC_HAS_REGEX is not set
# CONFIG_UCLIBC_HAS_FNMATCH is not set

# Wide char 支持
# CONFIG_UCLIBC_HAS_WCHAR is not set

# 數學庫（如果不需要）
# CONFIG_UCLIBC_HAS_FPU is not set
# CONFIG_UCLIBC_HAS_LIBM is not set

# 動態載入（使用靜態連結）
# CONFIG_UCLIBC_HAS_SHARED is not set
UCLIBC_STATIC_LDCONFIG=y

# GNU 擴展功能
# CONFIG_UCLIBC_HAS_GNU_ERROR is not set
# CONFIG_UCLIBC_HAS_GNU_GETOPT is not set
```

### 6. 優化 malloc 實作（節省 ~2-4 KB）

**配置選項**:
```makefile
MALLOC_SIMPLE=y                    # 使用簡單 malloc
# MALLOC_STANDARD is not set       # 不用標準 malloc
# MALLOC_GLIBC_COMPAT is not set

UCLIBC_HAS_MALLOC_SIMPLE=y
```

**影響**:
- ✗ malloc/free 性能較差
- ✗ 記憶體碎片可能增加
- ✓ 大幅減少 malloc 內部結構大小

## 建議的 uClibc-ng 最小化配置

### 配置檔案範例（.config）

```makefile
# 目標架構
TARGET_arm=y
TARGET_ARCH="arm"
TARGET_SUBARCH="cortex-m7"

# 不使用 MMU
ARCH_HAS_NO_MMU=y
UCLIBC_HAS_MMU=n

# 基本設定
UCLIBC_HAS_FLOATS=y
UCLIBC_HAS_FPU=n                   # Cortex-M7 可能有 FPU，依需求
DO_C99_MATH=y

# ===== 禁用 Locale（節省 ~30 KB）=====
# CONFIG_UCLIBC_HAS_LOCALE is not set
# CONFIG_UCLIBC_HAS_XLOCALE is not set

# ===== 禁用 Thread（節省 ~24 KB）=====
# CONFIG_UCLIBC_HAS_THREADS is not set
# CONFIG_UCLIBC_HAS_THREADS_NATIVE is not set

# ===== 減小 STDIO 緩衝（節省 ~6 KB）=====
UCLIBC_HAS_STDIO_BUFSIZ_256=y
# UCLIBC_HAS_STDIO_BUFSIZ_4096 is not set
UCLIBC_HAS_STDIO_BUILTIN_BUFFER_SIZE=256

# ===== 精簡錯誤訊息（節省 ~2 KB）=====
# CONFIG_UCLIBC_HAS_SYS_ERRLIST is not set
UCLIBC_HAS_ERRNO_MESSAGES=y
# CONFIG_UCLIBC_HAS_SYS_SIGLIST is not set

# ===== 禁用不需要功能（節省 ~8 KB）=====
# CONFIG_UCLIBC_HAS_IPV6 is not set
# CONFIG_UCLIBC_HAS_RPC is not set
# CONFIG_UCLIBC_HAS_WCHAR is not set
# CONFIG_UCLIBC_HAS_REGEX is not set       # 注意：會影響 grep 功能！

# ===== 簡化 malloc（節省 ~4 KB）=====
MALLOC_SIMPLE=y
# MALLOC_STANDARD is not set

# ===== 靜態連結 =====
# UCLIBC_HAS_SHARED is not set
UCLIBC_STATIC_LDCONFIG=y

# ===== 必須保留的功能（Toybox 需要）=====
UCLIBC_HAS_CTYPE_TABLES=y          # ctype.h 函數
UCLIBC_HAS_FNMATCH=y               # 檔案名稱匹配（ls, cp 需要）
UCLIBC_HAS_GLOB=y                  # 萬用字元展開
UCLIBC_HAS_GNU_GETOPT=y            # getopt_long（命令列解析）
UCLIBC_HAS_REALTIME=y              # POSIX realtime
UCLIBC_HAS_ADVANCED_REALTIME=n
```

## 編譯步驟

### 方法 1: 使用 Buildroot 重建工具鏈（推薦）

```bash
# 1. 下載 Buildroot（如果還沒有）
cd /home/rota1001/side-project/stm32h7-linux
wget https://buildroot.org/downloads/buildroot-2025.02.tar.gz
tar xzf buildroot-2025.02.tar.gz
cd buildroot-2025.02

# 2. 配置 Buildroot
make menuconfig

# 選擇：
# Target options ->
#   Target Architecture: ARM (little endian)
#   Target Architecture Variant: cortex-m7
#   Target ABI: FLAT (no MMU)
#
# Toolchain ->
#   C library: uClibc-ng
#   Custom uClibc config: [指向上面的最小化 .config]
#
# Target packages ->
#   (取消選擇所有，只編譯工具鏈)

# 3. 編譯工具鏈
make toolchain

# 4. 工具鏈會在 output/host/bin/ 目錄
export CROSS_COMPILE=$(pwd)/output/host/bin/arm-linux-
```

### 方法 2: 直接編譯 uClibc-ng（較複雜）

```bash
# 1. 下載 uClibc-ng
cd /home/rota1001/side-project/stm32h7-linux
git clone https://github.com/wbx-github/uclibc-ng.git
cd uclibc-ng

# 2. 複製最小化配置
cp /path/to/minimal.config .config

# 3. 調整配置
make ARCH=arm menuconfig

# 4. 編譯
make CROSS=arm-linux-gnueabi- ARCH=arm

# 5. 安裝
make CROSS=arm-linux-gnueabi- ARCH=arm PREFIX=/path/to/install install
```

## 預期結果

### RAM 使用量預測

```
原始 uClibc:     69.26 KB
- Locale         -29.00 KB
- Pthread        -24.00 KB
- STDIO buffers   -6.00 KB
- Error msgs      -2.00 KB
- 其他優化        -5.00 KB
----------------------------
最小化 uClibc:   ~3.26 KB  (僅保留 malloc, stdio 基本結構)

加上固定開銷:
  - ctype tables   2.25 KB
  - 基本 stdio     0.50 KB
  - malloc heap    2.00 KB
  - 其他系統       2.00 KB
----------------------------
實際 uClibc:    ~10.00 KB

Toybox 本身:      7.86 KB
----------------------------
總計:            ~18 KB    ✓ 遠低於 64 KB！
```

### 實際可能結果（保守估計）

考慮一些無法完全移除的開銷：
```
最小化 uClibc:   12-15 KB
Toybox:           7-8 KB
----------------------------
總計:            20-23 KB   ✓ 仍遠低於 64 KB！
```

## 風險與限制

### 高風險項目
1. **禁用 Locale** ⚠️ 
   - 檔名顯示可能有問題（非 ASCII 字符）
   - 排序行為改變

2. **禁用 Regex** ⚠️
   - 如果 Toybox 啟用 `grep`，則無法使用正則表達式
   - 建議保留 `UCLIBC_HAS_REGEX=y`

3. **禁用 Wide Char** ⚠️
   - UTF-8 檔名可能顯示錯誤
   - 建議至少保留基本 UTF-8 支持

### 低風險項目
1. **禁用 Thread** ✓
   - Toybox 不使用多執行緒
   - 完全安全

2. **減小 STDIO 緩衝** ✓
   - 僅影響性能，不影響功能
   - Toybox 有自己的緩衝

3. **簡化 malloc** ✓
   - Toybox 記憶體使用量小
   - 影響不大

## 建議的實施步驟

### 階段 1: 快速驗證（1-2 天）

1. **使用 Buildroot 快速重建工具鏈**
   - 僅禁用 Locale 和 Thread（最大節省）
   - 保留其他功能以確保相容性
   - 預期節省: ~50 KB → 目標 **27 KB**

2. **編譯 Toybox 並測試**
   - 驗證所有命令可正常運作
   - 測量實際 RAM 使用量

### 階段 2: 深度優化（3-5 天）

1. **進一步精簡**
   - 減小 STDIO 緩衝
   - 簡化 malloc
   - 精簡錯誤訊息
   - 預期節省: 額外 ~10 KB → 目標 **17-20 KB**

2. **測試所有 Toybox 命令**
   - 確保功能完整性
   - 檢查邊界情況

### 階段 3: 極限優化（可選，5-7 天）

1. **手動修改 uClibc-ng 源碼**
   - 移除未使用的符號表
   - 優化內部數據結構
   - 預期節省: 額外 ~2-3 KB

2. **Link-time optimization (LTO)**
   - 啟用 `-flto` 進行全程式優化
   - 可能額外節省 ~5-10%

## 替代方案

### 如果 uClibc-ng 優化仍不夠

1. **dietlibc** - 超輕量級 libc
   - RAM 使用: ~5-8 KB
   - 缺點: POSIX 不完整，需要修改 Toybox

2. **musl-libc** + XIP 模式
   - 如果可以讓 TEXT segment 在 Flash 執行（XIP）
   - 只計算 DATA+BSS 到 RAM

3. **自製 minimal libc**
   - 僅實作 Toybox 需要的函數
   - 工作量極大（2-3 個月）

## 結論

**是的，通過重新編譯 uClibc-ng 是完全可行的！**

### 預期成果
- **目前**: 77.12 KB（未達標）
- **優化後**: **18-25 KB**（遠低於 64 KB，甚至可能達到 32 KB！）

### 投資報酬率
- **時間投入**: 1-5 天
- **RAM 節省**: ~50-60 KB（77% 減少）
- **功能影響**: 最小（僅 locale 和 thread）

### 建議
**強烈建議進行 uClibc-ng 重新編譯**，因為：
1. 節省效果顯著（~60 KB）
2. 技術難度適中
3. Toybox 相容性良好
4. 可以達成原始目標（甚至超越！）

下一步可以開始準備 Buildroot 配置，需要幫忙嗎？
