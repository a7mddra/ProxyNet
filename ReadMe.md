# ProxyNet 🌐⚡

*A genome-terminal control panel that bridges your Ubuntu machine with Android's `HTTP Custom` app through ADB*

## ✨Key Features

### **GUI-Like Terminal Interface**
ProxyNet isn't just another CLI - it's a **terminal revolution** featuring:

- **Real-time frame rendering** (60fps smooth navigation)
- **Dynamic centering** that adapts to terminal resizing
- **Typewriter animations** (like LLMs token generation)
- **RGB color support** with ANSI escape artistry
- **Arrow-key navigation** with instant response
- **Atomic input handling** - no more input chaos during operations

### 🛠 **Core Functionality**
**Main Menu:**
- `Proxy Net` - Enter connection management panel
- `Settings` - Configure device and proxy settings  
- `Reset` - Disable Ubuntu proxy settings
- `Exit` - Graceful exit (no terminal freezing!)

**Connection Suite:**
- `Start Config` - Full automation: unlock → launch → connect
- `Connect ADB` - One-tap ADB wireless connection setup
- `Monitor` - Instant VPN/Proxy status check
- `Unlock` - PIN code phone unlocking

**Settings Powerhouse:**
- `View Data` - Inspect all configuration values
- `Edit Data` - Modify any setting with validation
- `Reset Settings` - Factory reset configuration
- `Edit Defaults` - Admin-only backup management

### 🔐 **Enterprise-Grade Security**
- **PAM Authentication** for sensitive operations
- **Config encryption-ready** architecture
- **Secure shell execution** with path validation
- **Atomic operation locking** prevents race conditions

## 🚀 Quick Start

### Installation
```bash
# Clone the repository
git clone https://github.com/a7mddra/ProxyNet.git
cd ProxyNet

# Install dependencies (Ubuntu)
make dep

# Build and run
make run
```

### Configuration
1. Navigate to `Settings` → `Edit Data`
2. Configure your values:

```ini
# Device Configuration
MDeviceHeight="1600"           # Measuring device height
MDeviceWidth="720"             # Measuring device width  
DeviceHeight=                  # Your actual device height
DeviceWidth=                   # Your actual device width
DevicePort=                    # ADB device port
DevicePIN=                     # Your phone PIN code
DeviceIP=                      # Your phone IP address
ApkX="250"                     # HTTP Custom splash X ratio
ApkY="840"                     # HTTP Custom splash Y ratio
ProxyURL=                      # SOCKS proxy URL
ProxyPort=                     # SOCKS proxy port
ConfigPath="assets/config.hc"  # HTTP Custom config file
```

3. **Admin features**: Use `Edit Defaults` to save your current configuration as factory backup (requires sudo)

## 🧠 How It Works

### ⚡ Multi-Core Parallel Processing
ProxyNet leverages your CPU's full potential by running operations across multiple cores simultaneously:

```cpp
// Dedicate Core 1 for buttery-smooth UI animations
CPU_ZERO(&cpuset);
CPU_SET(1, &cpuset);  // UI thread on core 1
pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);

// Dedicate Core 0 for heavy ADB operations  
CPU_ZERO(&cpuset);
CPU_SET(0, &cpuset);  // Worker thread on core 0
pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
```

### 🔄 **Intelligent Splash Detection**
ProxyNet uses pixel-perfect mathematics to detect when HTTP Custom is ready:

```cpp
// Dynamic coordinate calculation for any device
int x = (stoi(data.ApkX) * stoi(data.DeviceWidth)) / stoi(data.MDeviceWidth);
int y = (stoi(data.ApkY) * stoi(data.DeviceHeight)) / stoi(data.MDeviceHeight);
checkSplash(x, y);  // Waits for pixel color change
```

### 🐚 **Bash Integration Engine**
A sophisticated shell runner that securely executes scripts:

```cpp
// Safe shell execution with argument sanitization
shell.runShell("unlock", false, data.DevicePIN);
shell.runShell("adbd", true, data.DeviceIP, data.DevicePort); // New terminal window
```

### 📱 **ADB Automation Suite**
- **Smart connection management** with TCP reachability checks
- **Device state awareness** (locked/unlocked/ready)
- **Config push & import** automation
- **Input simulation** for seamless interaction

## 🎯 Perfect For

- **Developers** who need reliable proxy management
- **Privacy enthusiasts** using HTTP Custom regularly
- **Android power users** who want PC integration
- **Terminal lovers** who appreciate beautiful interfaces

## 📋 Requirements

- **Ubuntu** 20.04+ (other distros may work)
- **ADB** installed and configured
- **Android device** with USB debugging enabled
- **HTTP Custom** app installed on Android

## 🛠 Technical Architecture

```
ProxyNet/
├── src/               # C++ Core application
├── scripts/shell/     # Bash integration scripts
├── assets/            # Configuration files
│   ├── data.txt       # User configuration
│   ├── backup.txt     # Factory defaults (admin)
│   └── config.hc      # HTTP Custom configuration
└── Makefile           # Build system with dependencies
```

*⭐ Star this repo if ProxyNet makes your workflow awesome!*
