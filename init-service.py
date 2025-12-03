#!/usr/bin/env python3
import os
import sys
import time
import logging
import subprocess
from pathlib import Path

class InitService:
    def __init__(self):
        self.setup_logging()
        self.logger = logging.getLogger(__name__)
        
    def setup_logging(self):
        logging.basicConfig(
            level=logging.INFO,
            format='%(asctime)s - %(name)s - %(levelname)s - %(message)s',
            handlers=[
                logging.FileHandler('/var/log/init-service.log'),
                logging.StreamHandler()
            ]
        )
    
    def init_system(self):
        """执行系统初始化任务"""
        self.logger.info("开始系统初始化...")
        
        tasks = [
            self.create_directories,
            self.set_permissions,
            self.configure_system,
            self.start_dependencies
        ]
        
        for task in tasks:
            try:
                task()
                self.logger.info(f"任务 {task.__name__} 完成")
            except Exception as e:
                self.logger.error(f"任务 {task.__name__} 失败: {e}")
                return False
        
        self.logger.info("系统初始化完成")
        return True
    
    def create_directories(self):
        """创建必要的目录"""
        dirs = ['/var/lib/myapp', '/var/log/myapp', '/etc/myapp']
        for dir_path in dirs:
            Path(dir_path).mkdir(parents=True, exist_ok=True)
    
    def set_permissions(self):
        """设置文件权限"""
        os.chmod('/var/lib/myapp', 0o755)
        os.chmod('/var/log/myapp', 0o755)
    
    def configure_system(self):
        """配置系统参数"""
        # 示例：写入配置文件
        config_content = """# 系统配置
app_name=myapp
version=1.0
"""
        with open('/etc/myapp/config.conf', 'w') as f:
            f.write(config_content)
    
    def start_dependencies(self):
        """启动依赖服务"""
        # 示例：确保某些服务运行
        pass
    
    def run(self):
        """主运行方法"""
        self.logger.info("Init Service 启动")
        
        if self.init_system():
            self.logger.info("初始化成功，服务准备就绪")
            # 初始化完成后保持运行状态，等待Service Center管理
            while True:
                time.sleep(60)  # 每分钟检查一次状态
                self.logger.debug("服务运行中...")
        else:
            self.logger.error("初始化失败")
            sys.exit(1)

if __name__ == "__main__":
    service = InitService()
    try:
        service.run()
    except KeyboardInterrupt:
        service.logger.info("服务停止")
    except Exception as e:
        service.logger.error(f"服务异常: {e}")
        sys.exit(1)