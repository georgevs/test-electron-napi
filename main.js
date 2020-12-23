const { app, BrowserWindow } = require('electron');
const path = require('path');
const { testAsync, testSync } = require('@acme/addon');

console.log({ testSync: testSync() });
testAsync(x => console.log({ testAsync: x }));

function createWindow() {
  const win = new BrowserWindow({
    width: 1000,
    height: 800,
    webPreferences: {
      // https://www.electronjs.org/docs/tutorial/security
      nodeIntegration: false,  // default
      nodeIntegrationInWorker: false,  // default
      nodeIntegrationInSubFrames: false,
      enableRemoteModule: false,  // default
      webSecurity: true,  // default
      allowRunningInsecureContent: false,  // default
      contextIsolation: true,
      worldSafeExecuteJavaScript: true,
      webviewTag: false,
    }
  });

  win.loadFile(path.resolve(__dirname, './index.html'));
}

app.whenReady().then(createWindow);

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    app.quit();
  }
});

app.on('activate', () => {
  if (BrowserWindow.getAllWindows().length === 0) {
    createWindow();
  }
});
