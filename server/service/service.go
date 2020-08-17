package service

import (
    "context"
)

type ServerInfo struct {
    ServerPort      string
    ServerHost      string
}


type AppsServer interface {
    Start(ctx context.Context, address string) error
    Close(ctx context.Context) error
}

