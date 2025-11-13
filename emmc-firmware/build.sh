#!/bin/bash

# ====================================
# eMMC固件开发工程构建脚本
# ====================================

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# 打印带颜色的消息
print_info() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

# 显示帮助信息
show_help() {
    echo ""
    echo "eMMC固件开发工程 - 构建脚本"
    echo "======================================"
    echo "用法: ./build.sh [选项]"
    echo ""
    echo "选项:"
    echo "  all       - 编译所有目标（默认）"
    echo "  lib       - 仅编译静态库"
    echo "  test      - 编译并运行测试程序"
    echo "  clean     - 清理编译文件"
    echo "  debug     - 调试模式编译"
    echo "  help      - 显示此帮助信息"
    echo ""
}

# 编译静态库
build_lib() {
    print_info "开始编译eMMC固件库..."
    make clean
    make
    
    if [ $? -eq 0 ]; then
        print_info "库编译成功！"
        return 0
    else
        print_error "库编译失败！"
        return 1
    fi
}

# 编译测试程序
build_test() {
    print_info "开始编译测试程序..."
    make clean
    make test
    
    if [ $? -eq 0 ]; then
        print_info "测试程序编译成功！"
        return 0
    else
        print_error "测试程序编译失败！"
        return 1
    fi
}

# 运行测试
run_test() {
    build_test
    if [ $? -eq 0 ]; then
        print_info "运行测试程序..."
        make run-test
    fi
}

# 调试模式编译
build_debug() {
    print_info "开始调试模式编译..."
    make clean
    make DEBUG=1
    
    if [ $? -eq 0 ]; then
        print_info "调试版本编译成功！"
        return 0
    else
        print_error "调试版本编译失败！"
        return 1
    fi
}

# 清理
clean_all() {
    print_info "清理编译文件..."
    make clean
    print_info "清理完成！"
}

# 主程序
main() {
    case "$1" in
        lib)
            build_lib
            ;;
        test)
            run_test
            ;;
        clean)
            clean_all
            ;;
        debug)
            build_debug
            ;;
        help|-h|--help)
            show_help
            ;;
        all|"")
            build_lib
            ;;
        *)
            print_error "未知选项: $1"
            show_help
            exit 1
            ;;
    esac
}

# 执行主程序
main "$@"
