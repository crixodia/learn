import { Injectable } from '@angular/core';
import { ToastComponent, TOAST_TYPE } from '../../shared/components/toast/toast.component';

@Injectable({
  providedIn: 'root'
})
export class ToastService {
  private toastComponent: ToastComponent | null = null;
  private readonly DEFAULT_DURATION: number = 3000;

  registerToastComponent(toastComponent: ToastComponent): void {
    this.toastComponent = toastComponent;
  }

  showToast(title: string, message: string, type: string = TOAST_TYPE.success, duration: number = this.DEFAULT_DURATION): void {
    if (this.toastComponent) {
      this.toastComponent.toastTitle = title;
      this.toastComponent.toastMessage = message;
      this.toastComponent.toastType = type;
      this.toastComponent.toastShow = true;

      if (duration > 0) {
        setTimeout(() => {
          this.dismissToast();
        }, duration);
      }
    }
  }

  dismissToast(): void {
    if (this.toastComponent) {
      this.toastComponent.toastShow = false;
    }
  }
}
