import testNapi from 'libentry.so';
import XComponentContext from './XComponentContext';

const DOMAIN = 0x0000;

@Entry
@Component
struct Index {
  @State currentStatus: string = 'init';
  private xComponentContext: XComponentContext | undefined = undefined;
  build() {
    Row() {
      Column() {
        Column({ space: 10 }) {
          XComponent({
            id: 'xcomponentId',
            type: XComponentType.SURFACE,
            libraryname: 'entry'
          })
            .onLoad((xComponentContext) => {
              this.xComponentContext = xComponentContext as XComponentContext;
              this.currentStatus = 'index';
            })
            .onDestroy(() => {
              console.info('onDestroy');
            })
            .id('xcomponent')

          Text(this.currentStatus)
            .fontSize('24fp')
            .fontWeight(500)
        }
        .onClick(() => {
          let hasChangeColor: boolean = false;
          if (this.xComponentContext && this.xComponentContext.getStatusX()) {
            hasChangeColor = this.xComponentContext.getStatusX().hasChangeColor;
          }
          if (hasChangeColor) {
            this.currentStatus = 'change color';
          }
        })
        .margin({
          top: 27,
          left: 12,
          right: 12
        })
        .height('40%')
        .width('90%')

        XComponent({
          id: 'xcomponentId1',
          type: XComponentType.SURFACE,
          libraryname: 'entry'
        })
          .onLoad((xComponentContext) => {
          })
          .onDestroy(() => {
            console.info('onDestroy');
          })
          .id('xcomponent1')
          .width(100)
          .height(100)
        Row() {
          Button('Draw Star')
            .fontSize('16fp')
            .fontWeight(500)
            .margin({ bottom: 24 })
            .onClick(() => {
              if (this.xComponentContext) {
                this.xComponentContext.drawPatternX();
                let hasDraw: boolean = false;
                if (this.xComponentContext.getStatusX()) {
                  hasDraw = this.xComponentContext.getStatusX().hasDraw;
                }
                if (hasDraw) {
                  this.currentStatus = 'draw star';
                }
              }
            })
            .width('53.6%')
            .height(40)
        }
        .width('100%')
        .justifyContent(FlexAlign.Center)
        .alignItems(VerticalAlign.Bottom)
        .layoutWeight(1)
      }
      .width('100%')
    }
    .height('100%')
  }
}
