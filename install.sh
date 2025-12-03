#!/bin/bash

set -e

echo "安装 Init Service..."

# 复制服务文件
sudo cp init-service.py /usr/local/bin/
sudo chmod +x /usr/local/bin/init-service.py

# 复制服务配置
sudo cp init-service.service /etc/systemd/system/

# 重新加载systemd配置
sudo systemctl daemon-reload

# 启用服务
sudo systemctl enable init-service

echo "Init Service 安装完成"
echo "使用以下命令管理服务："
echo "  启动: sudo systemctl start init-service"
echo "  停止: sudo systemctl stop init-service"
echo "  状态: sudo systemctl status init-service"
echo "  日志: sudo journalctl -u init-service -f"