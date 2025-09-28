# AI_Cosplay

AI_Cosplay是一个基于C++和Web技术开发的AI角色扮演聊天应用，允许用户与不同角色的AI进行对话交流，体验角色扮演的乐趣。

## 项目特点

- 基于C++开发的高性能后端服务
- 简洁直观的Web前端界面
- 支持用户注册和登录功能
- 多种角色选择，满足不同的角色扮演需求
- 聊天历史记录保存功能

## 技术栈

- **后端**：C++17, Muduo网络库, OpenSSL
- **前端**：HTML, CSS, JavaScript
- **数据库**：MySQL
- **构建工具**：CMake

## 系统要求

- Linux操作系统（项目基于Linux开发和测试）
- C++17兼容的编译器（如GCC 7.0+）
- CMake 3.0或更高版本
- MySQL数据库服务器
- OpenSSL库
- Muduo网络库
- Boost库
- nlohmann/json库（用于JSON处理）

## 环境配置

### 1. 安装必要的依赖库

在Debian/Ubuntu系统上：

```bash
# 安装编译工具和基本库
sudo apt-get update
sudo apt-get install -y build-essential cmake

# 安装MySQL
sudo apt-get install -y mysql-server mysql-client libmysqlclient-dev

# 安装OpenSSL
sudo apt-get install -y libssl-dev

# 安装Boost库
sudo apt-get install -y libboost-all-dev

# 安装nlohmann/json库
sudo apt-get install -y nlohmann-json3-dev
```

### 2. 安装Muduo网络库

Muduo库需要手动编译安装：

```bash
git clone https://github.com/chenshuo/muduo.git
cd muduo
mkdir build && cd build
cmake ..
make -j4
sudo make install
```

## 安装与使用

### 1. 克隆仓库

```bash
git clone https://github.com/Sheffy001/AI_Cosplay.git
cd AI_Cosplay
```

### 2. 配置数据库

1. 创建MySQL数据库：

在命令行中执行，导入数据库表结构：

```bash
mysql -u root -p mydb < /home/user/project/mytables.sql

-- 创建其他必要的表（根据需要）
```

2. 配置数据库连接信息：

项目使用`bin/mysql.ini`文件配置数据库连接信息，默认配置如下：

```ini
username=root       # 数据库用户名
password=root       # 数据库密码
dbname=aichat       # 数据库名称
ip=127.0.0.1        # 数据库服务器IP
port=3306           # 数据库端口
maxSize=1024        # 连接池最大连接数
initSize=10         # 连接池初始连接数
maxIdleTime=60      # 最大空闲时间（秒）
connectionTimeout=100 # 连接超时时间（毫秒）
```

请根据您的实际数据库配置修改这些参数。

### 3. 编译项目

```bash
cd build
make
```

### 4. 运行服务器

```bash
cd ../bin
./aiserver -p 80
```

默认情况下，服务器将在80端口启动。可以使用`-p`参数指定其他端口。

### 5. 访问应用

在浏览器中访问 `http://localhost:80` 或您指定的端口。

## 项目结构

- `aichat/`: 聊天服务器核心代码
- `HttpServer/`: HTTP服务器实现
- `tools/`: 工具类和数据库连接池
- `source/`: 前端页面和资源
- `bin/`: 编译后的可执行文件和配置文件
- `lib/`: 编译后的库文件

## 功能说明

1. **用户管理**：支持用户注册、登录和注销
2. **角色选择**：提供多种AI角色供用户选择
3. **聊天功能**：与所选角色进行实时对话
4. **历史记录**：保存和查看历史聊天记录
5. **角色故事**：每个角色都有独特的背景故事

## 常见问题

1. **编译错误**：确保已安装所有必要的依赖库，并且版本兼容
2. **数据库连接失败**：检查`mysql.ini`配置是否正确，MySQL服务是否启动
3. **端口被占用**：使用`-p`参数指定其他可用端口

## 贡献指南

欢迎提交问题和功能请求！如果您想贡献代码，请遵循以下步骤：

1. Fork 仓库
2. 创建您的特性分支 (`git checkout -b feature/amazing-feature`)
3. 提交您的更改 (`git commit -m 'Add some amazing feature'`)
4. 推送到分支 (`git push origin feature/amazing-feature`)
5. 打开一个 Pull Request

## 许可证

本项目采用 MIT 许可证 - 详情请参阅 [LICENSE](LICENSE) 文件。

## 联系方式

如有任何问题或建议，请通过以下方式联系我们：

- 项目维护者：[xie fei](mailto:1980278709@qq.com)
- 项目仓库：[GitHub](https://github.com/Sheffy001/AI_Cosplay#)