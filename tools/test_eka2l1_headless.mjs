import fs from 'node:fs';
import path from 'node:path';
import net from 'node:net';
import { spawn } from 'node:child_process';
import { fileURLToPath } from 'node:url';

const scriptDir = path.dirname(fileURLToPath(import.meta.url));
const rootDir = path.resolve(scriptDir, '..');
const defaultChrome = process.platform === 'darwin'
    ? '/Applications/Google Chrome.app/Contents/MacOS/Google Chrome'
    : 'google-chrome';

function defaultGame() {
    const directory = path.join(rootDir, 'games');
    const games = fs.readdirSync(directory)
        .filter(name => name.toLowerCase().endsWith('.zip'))
        .sort((left, right) => left.localeCompare(right));
    if (games.length === 0) throw new Error(`No ZIP games found in ${directory}`);
    return path.join(directory, games[0]);
}

function parseArgs(argv) {
    const out = {
        site: 'https://eka2l1.zixing.fun/',
        chrome: process.env.EKA2L1_CHROME || defaultChrome,
        rom: path.join(rootDir, 'games', 'E5 (S60v3)', 'E5.rom'),
        rpkg: path.join(rootDir, 'games', 'E5 (S60v3)', 'SYM.RPKG'),
        sis: path.join(rootDir, 'sis', 'EasyRPG.sis'),
        game: defaultGame(),
        profile: path.join(rootDir, 'build', 'eka2l1-headless-profile'),
        output: path.join(rootDir, 'build', 'eka2l1-headless'),
        uid: 0xE31EA5A1,
        duration: 15,
        launchTimeout: 120,
        keySequence: '',
        keyDelay: 250,
        keyWait: 1000,
        keysAfterScene: '',
        noStartGame: false,
        enterGame: false,
        resetProfile: false
    };

    for (let i = 0; i < argv.length; ++i) {
        const arg = argv[i];
        const take = () => {
            if (i + 1 >= argv.length) throw new Error(`Missing value for ${arg}`);
            return argv[++i];
        };
        if (arg === '--site') out.site = take();
        else if (arg === '--chrome') out.chrome = take();
        else if (arg === '--rom') out.rom = path.resolve(take());
        else if (arg === '--rpkg') out.rpkg = path.resolve(take());
        else if (arg === '--sis') out.sis = path.resolve(take());
        else if (arg === '--game') out.game = path.resolve(take());
        else if (arg === '--profile') out.profile = path.resolve(take());
        else if (arg === '--output') out.output = path.resolve(take());
        else if (arg === '--uid') out.uid = Number(take());
        else if (arg === '--duration') out.duration = Number(take());
        else if (arg === '--launch-timeout') out.launchTimeout = Number(take());
        else if (arg === '--keys') out.keySequence = take();
        else if (arg === '--key-delay') out.keyDelay = Number(take());
        else if (arg === '--keys-wait') out.keyWait = Number(take());
        else if (arg === '--keys-after-scene') out.keysAfterScene = take();
        else if (arg === '--no-start-game') out.noStartGame = true;
        else if (arg === '--enter-game') out.enterGame = true;
        else if (arg === '--reset-profile') out.resetProfile = true;
        else if (arg === '-h' || arg === '--help') out.help = true;
        else throw new Error(`Unknown argument: ${arg}`);
    }
    return out;
}

function usage() {
    process.stdout.write([
        'Usage: node tools/test_eka2l1_headless.mjs [options]',
        '',
        'Options:',
        '  --rom PATH',
        '  --rpkg PATH',
        '  --sis PATH',
        '  --game PATH',
        '  --chrome PATH',
        '  --site URL',
        '  --profile PATH',
        '  --output PATH',
        '  --uid NUMBER',
        '  --duration SECONDS',
        '  --launch-timeout SECONDS',
        '  --keys NAME[,NAME...] or NAME*COUNT',
        '  --key-delay MILLISECONDS',
        '  --keys-wait MILLISECONDS',
        '  --keys-after-scene NAME',
        '  --no-start-game',
        '  --enter-game',
        '  --reset-profile',
        ''
    ].join('\n'));
}

function assertFile(file, name) {
    if (!fs.existsSync(file) || !fs.statSync(file).isFile()) {
        throw new Error(`${name} not found: ${file}`);
    }
}

function sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}

function appExitKey(exit) {
    if (!exit) return '';
    return `${exit.uid}:${exit.type}:${exit.reason}:${exit.category}:${exit.name}`;
}

async function freePort() {
    return await new Promise((resolve, reject) => {
        const server = net.createServer();
        server.once('error', reject);
        server.listen(0, '127.0.0.1', () => {
            const port = server.address().port;
            server.close(err => err ? reject(err) : resolve(port));
        });
    });
}

async function waitHttp(url, timeoutMs) {
    const deadline = Date.now() + timeoutMs;
    let lastError;
    while (Date.now() < deadline) {
        try {
            const response = await fetch(url);
            if (response.ok) return await response.json();
        } catch (error) {
            lastError = error;
        }
        await sleep(100);
    }
    throw new Error(`Timed out waiting for ${url}: ${lastError || 'no response'}`);
}

class Cdp {
    constructor(url) {
        this.url = url;
        this.ws = null;
        this.nextId = 1;
        this.pending = new Map();
        this.listeners = new Map();
        this.closedError = null;
    }

    async connect() {
        this.ws = new WebSocket(this.url);
        await new Promise((resolve, reject) => {
            const onOpen = () => {
                this.ws.removeEventListener('error', onError);
                resolve();
            };
            const onError = event => {
                this.ws.removeEventListener('open', onOpen);
                reject(new Error(`DevTools websocket error: ${event.message || 'connection failed'}`));
            };
            this.ws.addEventListener('open', onOpen, { once: true });
            this.ws.addEventListener('error', onError, { once: true });
        });
        this.ws.addEventListener('message', event => this.handleMessage(event.data));
        this.ws.addEventListener('close', () => this.handleClose(new Error('DevTools websocket closed')));
        this.ws.addEventListener('error', event => this.handleClose(new Error(`DevTools websocket error: ${event.message || 'unknown error'}`)));
    }

    async handleMessage(data) {
        let text;
        if (typeof data === 'string') text = data;
        else if (data instanceof ArrayBuffer) text = Buffer.from(data).toString('utf8');
        else if (ArrayBuffer.isView(data)) text = Buffer.from(data.buffer, data.byteOffset, data.byteLength).toString('utf8');
        else if (data && typeof data.text === 'function') text = await data.text();
        else text = String(data);
        const message = JSON.parse(text);
        if (message.id) {
            const pending = this.pending.get(message.id);
            if (!pending) return;
            this.pending.delete(message.id);
            if (message.error) pending.reject(new Error(`${message.error.message}${message.error.data ? `: ${message.error.data}` : ''}`));
            else pending.resolve(message.result);
            return;
        }
        const handlers = this.listeners.get(message.method) || [];
        for (const handler of handlers) handler(message.params || {});
    }

    handleClose(error) {
        if (this.closedError) return;
        this.closedError = error;
        for (const pending of this.pending.values()) pending.reject(error);
        this.pending.clear();
    }

    on(method, handler) {
        const handlers = this.listeners.get(method) || [];
        handlers.push(handler);
        this.listeners.set(method, handlers);
    }

    call(method, params = {}) {
        if (this.closedError) return Promise.reject(this.closedError);
        const id = this.nextId++;
        return new Promise((resolve, reject) => {
            this.pending.set(id, { resolve, reject });
            this.ws.send(JSON.stringify({ id, method, params }));
        });
    }

    close() {
        if (this.ws && this.ws.readyState <= WebSocket.OPEN) this.ws.close();
    }
}

function remoteText(remote) {
    if (!remote) return '';
    if (Object.prototype.hasOwnProperty.call(remote, 'value')) {
        if (typeof remote.value === 'string') return remote.value;
        try { return JSON.stringify(remote.value); } catch {}
    }
    return remote.description || remote.type || '';
}

async function main() {
    const options = parseArgs(process.argv.slice(2));
    if (options.help) {
        usage();
        return;
    }

    assertFile(options.sis, 'SIS');
    if (path.isAbsolute(options.chrome)) assertFile(options.chrome, 'Chrome');

    if (options.resetProfile) fs.rmSync(options.profile, { recursive: true, force: true });
    fs.mkdirSync(options.profile, { recursive: true });
    fs.rmSync(options.output, { recursive: true, force: true });
    fs.mkdirSync(options.output, { recursive: true });

    const port = await freePort();
    const chromeArgs = [
        '--headless=new',
        `--remote-debugging-port=${port}`,
        `--user-data-dir=${options.profile}`,
        '--no-first-run',
        '--no-default-browser-check',
        '--disable-background-networking',
        '--disable-component-update',
        '--disable-sync',
        '--disable-features=Translate,MediaRouter',
        '--autoplay-policy=no-user-gesture-required',
        '--use-gl=angle',
        '--use-angle=swiftshader-webgl',
        '--enable-unsafe-swiftshader',
        '--window-size=960,720',
        'about:blank'
    ];

    const chrome = spawn(options.chrome, chromeArgs, { stdio: ['ignore', 'pipe', 'pipe'] });
    let chromeStderr = '';
    chrome.stderr.on('data', chunk => { chromeStderr += chunk.toString(); });
    chrome.stdout.on('data', chunk => { chromeStderr += chunk.toString(); });

    let cdp;
    const consoleLines = [];
    const result = {
        site: options.site,
        uid: options.uid >>> 0,
        rom: options.rom,
        rpkg: options.rpkg,
        sis: options.sis,
        game: options.game,
        startedAt: new Date().toISOString(),
        deviceInstalled: false,
        sisInstalled: false,
        gameUploaded: false,
        firstFrame: false,
        appExit: null,
        redrawFps: 0,
        guestLog: null,
        guestFs: null,
        screenshot: null,
        success: false
    };

    const pushLog = (line, echo = true) => {
        const value = `[${new Date().toISOString()}] ${line}`;
        consoleLines.push(value);
        if (echo) process.stdout.write(`${value}\n`);
    };

    let crashed = false;
    let monitorGuestFatal = false;
    let guestFatal = '';
    let fatalTrace = null;
    let cachedTrace = null;
    let lastTracePoll = 0;

    try {
        await waitHttp(`http://127.0.0.1:${port}/json/version`, 15000);
        const targets = await waitHttp(`http://127.0.0.1:${port}/json/list`, 5000);
        const target = targets.find(item => item.type === 'page');
        if (!target) throw new Error('Chrome did not expose a page target');

        cdp = new Cdp(target.webSocketDebuggerUrl);
        await cdp.connect();
        await Promise.all([
            cdp.call('Page.enable'),
            cdp.call('Runtime.enable'),
            cdp.call('Log.enable'),
            cdp.call('DOM.enable'),
            cdp.call('Inspector.enable')
        ]);

        cdp.on('Runtime.consoleAPICalled', params => {
            const text = params.args.map(remoteText).join(' ');
            pushLog(`console.${params.type}: ${text}`, false);
            if (monitorGuestFatal && !guestFatal &&
                (text.includes('Access violation reading address') ||
                 text.includes('Panic context:') ||
                 text.includes('Thread Main panicked') ||
                 text.includes('KERN-EXEC') ||
                 text.includes('E32USER-CBase'))) {
                guestFatal = text;
                // The page may become unusable right after the fatal; grab the
                // rgss.trace snapshot now through a direct CDP call.
                cdp.call('Runtime.evaluate', {
                    expression: `EKA2L1.module.FS.readFile('/eka2l1/drives/c/data/easyrpg/rgss.trace', { encoding: 'utf8' })`,
                    returnByValue: true
                }).then(response => {
                    const value = response && response.result && response.result.value;
                    if (typeof value === 'string' && value.length) {
                        fatalTrace = value;
                    }
                }).catch(() => {});
            }
        });
        cdp.on('Runtime.exceptionThrown', params => {
            const detail = params.exceptionDetails || {};
            pushLog(`exception: ${detail.text || remoteText(detail.exception)}`);
        });
        cdp.on('Log.entryAdded', params => {
            const entry = params.entry || {};
            pushLog(`log.${entry.level || 'info'}: ${entry.text || ''}`, false);
        });
        cdp.on('Page.frameNavigated', params => {
            if (params.frame?.parentId) return;
            pushLog(`frame: ${params.frame?.url || ''}`, false);
        });
        cdp.on('Inspector.targetCrashed', () => {
            crashed = true;
            pushLog('browser target crashed');
        });

        const evaluate = async (expression, awaitPromise = false) => {
            if (crashed) throw new Error('Browser target crashed');
            if (guestFatal) throw new Error(`Guest runtime fatal: ${guestFatal}`);
            const response = await cdp.call('Runtime.evaluate', {
                expression,
                awaitPromise,
                returnByValue: true,
                userGesture: true
            });
            if (response.exceptionDetails) {
                const detail = response.exceptionDetails;
                throw new Error(detail.exception?.description || detail.text || 'JavaScript evaluation failed');
            }
            return response.result?.value;
        };

        const waitEval = async (expression, timeoutMs, label) => {
            const deadline = Date.now() + timeoutMs;
            let lastValue;
            let lastError;
            while (Date.now() < deadline) {
                if (crashed) throw new Error('Browser target crashed');
                try {
                    lastValue = await evaluate(expression);
                    if (lastValue) return lastValue;
                } catch (error) {
                    lastError = error;
                }
                await sleep(250);
            }
            throw new Error(`Timed out waiting for ${label}${lastError ? `: ${lastError.message}` : ''}${lastValue ? ` (${JSON.stringify(lastValue)})` : ''}`);
        };

        const navigate = async url => {
            pushLog(`navigate: ${url}`);
            await cdp.call('Page.navigate', { url });
            await waitEval("document.readyState === 'complete'", 30000, 'page load');
        };

        const setInputFiles = async (selector, files) => {
            const doc = await cdp.call('DOM.getDocument', { depth: 2, pierce: true });
            const found = await cdp.call('DOM.querySelector', { nodeId: doc.root.nodeId, selector });
            if (!found.nodeId) throw new Error(`Input not found: ${selector}`);
            await cdp.call('DOM.setFileInputFiles', { nodeId: found.nodeId, files });
        };

        const capture = async name => {
            const image = await cdp.call('Page.captureScreenshot', {
                format: 'png',
                fromSurface: true,
                captureBeyondViewport: true
            });
            const file = path.join(options.output, name);
            fs.writeFileSync(file, Buffer.from(image.data, 'base64'));
            result.screenshot = file;
            return file;
        };

        const readIndexState = async () => await evaluate(`(() => {
            const error = document.getElementById('bootError');
            const dot = document.getElementById('coreStatusDot');
            const status = document.getElementById('coreStatusText');
            return {
                hasDevice: localStorage.getItem('eka2l1_has_device') === '1',
                ready: !!(window.EKA2L1 && EKA2L1.ready),
                green: !!(dot && dot.classList.contains('green')),
                red: !!(dot && dot.classList.contains('red')),
                status: status ? status.textContent : '',
                error: error && error.style.display !== 'none' ? error.textContent : ''
            };
        })()`);

        await navigate(options.site);
        await waitEval("!!(window.EKA2L1 && document.getElementById('romInput') && document.getElementById('rpkgInput'))", 30000, 'EKA2L1 library UI');
        await waitEval("!('serviceWorker' in navigator) || !!navigator.serviceWorker.controller", 30000, 'service worker control');
        await waitEval("!!(window.EKA2L1 && document.getElementById('romInput') && document.getElementById('rpkgInput'))", 30000, 'stable EKA2L1 library UI');
        await sleep(750);
        const environment = await evaluate(`(() => {
            const canvas = document.createElement('canvas');
            const gl = canvas.getContext('webgl2') || canvas.getContext('webgl');
            let renderer = '';
            if (gl) {
                const ext = gl.getExtension('WEBGL_debug_renderer_info');
                if (ext) renderer = gl.getParameter(ext.UNMASKED_RENDERER_WEBGL) || '';
            }
            return {
                crossOriginIsolated: self.crossOriginIsolated,
                sharedArrayBuffer: typeof SharedArrayBuffer !== 'undefined',
                webgl: !!gl,
                renderer
            };
        })()`);
        pushLog(`environment: ${JSON.stringify(environment)}`);

        let indexState = await readIndexState();
        if (!indexState.hasDevice) {
            assertFile(options.rom, 'ROM');
            assertFile(options.rpkg, 'RPKG');
            pushLog('device: installing ROM/RPKG into persistent headless profile');
            await setInputFiles('#romInput', [options.rom]);
            await evaluate("document.getElementById('romInput').dispatchEvent(new Event('change', {bubbles:true}))");
            await setInputFiles('#rpkgInput', [options.rpkg]);
            await evaluate("document.getElementById('rpkgInput').dispatchEvent(new Event('change', {bubbles:true}))");
            await evaluate('window.installDevice()');
            const deadline = Date.now() + 300000;
            while (Date.now() < deadline) {
                indexState = await readIndexState();
                if (indexState.error || indexState.red) throw new Error(`Device install failed: ${indexState.error || indexState.status}`);
                if (indexState.hasDevice && indexState.ready && indexState.green) break;
                await sleep(500);
            }
            if (!(indexState.hasDevice && indexState.ready && indexState.green)) {
                throw new Error(`Device install did not complete: ${JSON.stringify(indexState)}`);
            }
        } else {
            pushLog('device: reusing persistent ROM/RPKG profile');
            const deadline = Date.now() + 180000;
            while (Date.now() < deadline) {
                indexState = await readIndexState();
                if (indexState.error || indexState.red) throw new Error(`Device boot failed: ${indexState.error || indexState.status}`);
                if (indexState.ready && indexState.green) break;
                await sleep(500);
            }
            if (!(indexState.ready && indexState.green)) throw new Error(`Device boot did not complete: ${JSON.stringify(indexState)}`);
        }
        result.deviceInstalled = true;
        pushLog('device: ready');

        const uninstallResult = await evaluate(`(() => {
            const uid = ${options.uid >>> 0};
            const apps = EKA2L1.appList();
            if (!apps.some(app => (app.uid >>> 0) === uid)) return null;
            return EKA2L1.module.ccall('wasm_uninstall_package', 'number', ['number'], [uid]);
        })()`);
        if (uninstallResult !== null) {
            if (uninstallResult !== 0) throw new Error(`Existing EasyRPG uninstall failed: ${uninstallResult}`);
            pushLog('sis: removed previous EasyRPG package');
        }

        await evaluate(`(() => {
            const old = document.getElementById('headlessSisInput');
            if (old) old.remove();
            const input = document.createElement('input');
            input.type = 'file';
            input.id = 'headlessSisInput';
            input.style.display = 'none';
            document.body.appendChild(input);
            return true;
        })()`);
        await setInputFiles('#headlessSisInput', [options.sis]);
        const sisResult = await evaluate(`(async () => {
            const input = document.getElementById('headlessSisInput');
            const file = input.files[0];
            if (!file) throw new Error('SIS file input is empty');
            const target = '/eka2l1/temp/headless-easyrpg.sis';
            await EKA2L1.writeFileToVFS(file, target);
            const code = EKA2L1.installPackage(target);
            try { EKA2L1.module.FS.unlink(target); } catch (e) {}
            if (code !== 0) return { code, apps: EKA2L1.appList() };
            await EKA2L1.save();
            return { code, apps: EKA2L1.appList() };
        })()`, true);
        if (!sisResult || sisResult.code !== 0) throw new Error(`SIS install failed: ${JSON.stringify(sisResult)}`);
        if (!sisResult.apps.some(app => (app.uid >>> 0) === (options.uid >>> 0))) {
            throw new Error(`SIS installed but UID 0x${(options.uid >>> 0).toString(16)} is absent from app list`);
        }
        result.sisInstalled = true;
        pushLog(`sis: installed UID 0x${(options.uid >>> 0).toString(16)}`);

        if (fs.existsSync(options.game) && fs.statSync(options.game).isFile()) {
            await evaluate(`(() => {
                const old = document.getElementById('headlessGameInput');
                if (old) old.remove();
                const input = document.createElement('input');
                input.type = 'file';
                input.id = 'headlessGameInput';
                input.style.display = 'none';
                document.body.appendChild(input);
                return true;
            })()`);
            await setInputFiles('#headlessGameInput', [options.game]);
            const guestFileName = path.basename(options.game).toLowerCase();
            const guestFsPath = `/eka2l1/drives/c/data/easyrpg/games/${guestFileName}`;
            const uploadResult = await evaluate(`(async () => {
                const input = document.getElementById('headlessGameInput');
                const file = input.files[0];
                if (!file) throw new Error('Game file input is empty');
                const dir = '/eka2l1/drives/c/data/easyrpg/games';
                const target = ${JSON.stringify(guestFsPath)};
                EKA2L1.module.FS.mkdirTree(dir);
                for (const name of EKA2L1.module.FS.readdir(dir)) {
                    if (name === '.' || name === '..') continue;
                    const existing = dir + '/' + name;
                    try {
                        const st = EKA2L1.module.FS.stat(existing);
                        if (EKA2L1.module.FS.isFile(st.mode)) EKA2L1.module.FS.unlink(existing);
                    } catch (e) {}
                }
                await EKA2L1.writeFileToVFS(file, target);
                await EKA2L1.save();
                const st = EKA2L1.module.FS.stat(target);
                return { size: st.size, target };
            })()`, true);
            const localGameSize = fs.statSync(options.game).size;
            if (!uploadResult || uploadResult.size !== localGameSize) {
                throw new Error(`Game upload size mismatch: local=${localGameSize} remote=${uploadResult?.size}`);
            }
            result.gameUploaded = true;
            pushLog(`game: uploaded ${uploadResult.target} (${uploadResult.size} bytes)`);
        } else {
            const existingGame = await evaluate(`(() => {
                const dir = '/eka2l1/drives/c/data/easyrpg/games';
                try {
                    const files = EKA2L1.module.FS.readdir(dir).filter(name => {
                        if (name === '.' || name === '..') return false;
                        try {
                            const st = EKA2L1.module.FS.stat(dir + '/' + name);
                            return EKA2L1.module.FS.isFile(st.mode);
                        } catch (e) { return false; }
                    });
                    if (!files.length) return null;
                    const target = dir + '/' + files[0];
                    return { target, size: EKA2L1.module.FS.stat(target).size };
                } catch (e) { return null; }
            })()`);
            if (!existingGame)
                throw new Error(`Game zip not found locally and profile has no guest game: ${options.game}`);
            result.gameUploaded = true;
            pushLog(`game: reusing ${existingGame.target} (${existingGame.size} bytes)`);
        }

        const siteUrl = new URL(options.site);
        const runUrl = new URL('run.html', siteUrl);
        for (const [key, value] of siteUrl.searchParams) runUrl.searchParams.set(key, value);
        runUrl.searchParams.set('uid', String(options.uid >>> 0));
        runUrl.searchParams.set('name', 'EasyRPG Player');
        await navigate(runUrl.toString());
        await waitEval('!!(window.EKA2L1 && document.getElementById("canvas"))', 30000, 'EKA2L1 runner UI');

        const launchDeadline = Date.now() + options.launchTimeout * 1000;
        let launchState = null;
        while (Date.now() < launchDeadline) {
            launchState = await evaluate(`(() => {
                if (!window.EKA2L1 || !EKA2L1.module) return { ready: false, redraw: 0, exit: null };
                return {
                    ready: !!EKA2L1.ready,
                    redraw: EKA2L1.redrawCount ? EKA2L1.redrawCount() : 0,
                    exit: EKA2L1.lastAppExit ? EKA2L1.lastAppExit() : null
                };
            })()`);
            if (launchState.exit) {
                result.appExit = launchState.exit;
                throw new Error(`App exited before first frame: ${JSON.stringify(launchState.exit)}`);
            }
            if (launchState.redraw > 0) break;
            await sleep(250);
        }
        if (!launchState || launchState.redraw <= 0) throw new Error(`No composed frame after ${options.launchTimeout}s`);
        result.firstFrame = true;
        pushLog(`app: first frame, redraw=${launchState.redraw}`);
        await capture('app-first-frame.png');

        let handoffExitKey = '';
        await sleep(1200);
        if (!options.noStartGame) {
            monitorGuestFatal = true;
            await evaluate(`(async () => {
                const code = EKA2L1.keys.OK;
                EKA2L1.sendKey(code, 1);
                await new Promise(resolve => setTimeout(resolve, 90));
                EKA2L1.sendKey(code, 0);
                return true;
            })()`, true);
            pushLog('app: sent OK to launch selected game');
            const handoffDeadline = Date.now() + options.launchTimeout * 1000;
            while (Date.now() < handoffDeadline) {
                const exit = await evaluate('EKA2L1.module && EKA2L1.lastAppExit ? EKA2L1.lastAppExit() : null');
                if (exit) {
                    if ((exit.uid >>> 0) !== (options.uid >>> 0) || exit.type !== 0 || exit.reason !== 0) {
                        result.appExit = exit;
                        throw new Error(`App exited during handoff: ${JSON.stringify(exit)}`);
                    }
                    handoffExitKey = appExitKey(exit);
                    pushLog(`app: launcher handed off to runtime, exit=${exit.reason}`);
                    break;
                }
                await sleep(100);
            }
            if (!handoffExitKey)
                throw new Error(`Runtime handoff did not complete within ${options.launchTimeout}s`);
        }
        if (options.enterGame && !options.noStartGame) {
            await sleep(2000);
            await evaluate(`(async () => {
                const code = EKA2L1.keys.OK;
                EKA2L1.sendKey(code, 1);
                await new Promise(resolve => setTimeout(resolve, 90));
                EKA2L1.sendKey(code, 0);
                return true;
            })()`, true);
            pushLog('app: sent OK on title');
            await sleep(1000);
        }

        if (options.keySequence) {
            if (options.keysAfterScene) {
                const wanted = options.keysAfterScene.toLowerCase();
                const sceneDeadline = Date.now() + options.launchTimeout * 1000;
                let seen = false;
                while (Date.now() < sceneDeadline) {
                    let traceText = '';
                    try {
                        traceText = await evaluate(`EKA2L1.module.FS.readFile('/eka2l1/drives/c/data/easyrpg/rgss.trace', { encoding: 'utf8' })`);
                    } catch (error) {}
                    if (typeof traceText === 'string' && traceText.toLowerCase().includes(wanted)) {
                        seen = true;
                        break;
                    }
                    await sleep(500);
                }
                if (seen) pushLog(`keys-after-scene: scene "${options.keysAfterScene}" observed`);
                else pushLog(`keys-after-scene: scene "${options.keysAfterScene}" not observed, sending keys anyway`);
                await sleep(1000);
            } else {
                await sleep(options.keyWait);
            }
            const sequence = [];
            for (const rawToken of options.keySequence.split(',')) {
                const token = rawToken.trim();
                if (!token) continue;
                const match = /^([A-Za-z0-9_]+)(?:\*(\d+))?$/.exec(token);
                if (!match) throw new Error(`Invalid key token: ${token}`);
                const count = match[2] ? Number(match[2]) : 1;
                if (!Number.isInteger(count) || count < 1 || count > 100) {
                    throw new Error(`Invalid key repeat count: ${token}`);
                }
                for (let repeat = 0; repeat < count; ++repeat) sequence.push(match[1].toUpperCase());
            }
            for (const name of sequence) {
                await evaluate(`(async () => {
                    const name = ${JSON.stringify(name)};
                    const code = EKA2L1.keys[name];
                    if (code === undefined) throw new Error('Unknown EKA2L1 key: ' + name + '; available=' + Object.keys(EKA2L1.keys).sort().join(','));
                    EKA2L1.sendKey(code, 1);
                    await new Promise(resolve => setTimeout(resolve, 90));
                    EKA2L1.sendKey(code, 0);
                    return true;
                })()`, true);
                pushLog(`app: sent key ${name}`);
                await sleep(options.keyDelay);
            }
        }

        const sampleStart = Date.now();
        const redrawStart = await evaluate('EKA2L1.module && EKA2L1.redrawCount ? EKA2L1.redrawCount() : 0');
        const testDeadline = sampleStart + options.duration * 1000;
        while (Date.now() < testDeadline) {
            const state = await evaluate(`({
                redraw: EKA2L1.module && EKA2L1.redrawCount ? EKA2L1.redrawCount() : 0,
                exit: EKA2L1.module && EKA2L1.lastAppExit ? EKA2L1.lastAppExit() : null
            })`);
            if (Date.now() - lastTracePoll > 2000) {
                lastTracePoll = Date.now();
                try {
                    cachedTrace = await evaluate(`EKA2L1.module.FS.readFile('/eka2l1/drives/c/data/easyrpg/rgss.trace', { encoding: 'utf8' })`);
                } catch (error) {}
            }
            if (state.exit && appExitKey(state.exit) !== handoffExitKey) {
                const cleanLauncherHandoff = (state.exit.uid >>> 0) === (options.uid >>> 0) && state.exit.type === 0 && state.exit.reason === 0;
                if (cleanLauncherHandoff && !handoffExitKey) {
                    handoffExitKey = appExitKey(state.exit);
                    pushLog(`app: launcher handed off to runtime late, exit=${state.exit.reason}`);
                } else {
                    result.appExit = state.exit;
                    if (cachedTrace !== null) {
                        result.guestLog = cachedTrace;
                        fs.writeFileSync(path.join(options.output, 'rgss.trace'), cachedTrace);
                    }
                    throw new Error(`App exited during test: ${JSON.stringify(state.exit)}`);
                }
            }
            await sleep(500);
        }
        const sampleEnd = Date.now();
        const redrawEnd = await evaluate('EKA2L1.redrawCount ? EKA2L1.redrawCount() : 0');
        result.redrawFps = (redrawEnd - redrawStart) * 1000 / Math.max(1, sampleEnd - sampleStart);
        result.guestFs = await evaluate(`(() => {
            const fs = EKA2L1.module.FS;
            const paths = ['/', '/eka2l1', '/eka2l1/drives', '/eka2l1/drives/c', '/eka2l1/drives/c/data', '/eka2l1/drives/c/data/easyrpg', '/eka2l1/drives/c/data/easyrpg/saves'];
            const out = {};
            for (const target of paths) {
                try { out[target] = fs.readdir(target); }
                catch (error) { out[target] = String(error); }
            }
            return out;
        })()`);
        pushLog(`guest-fs: ${JSON.stringify(result.guestFs)}`);
        try {
            result.guestLog = await evaluate(`EKA2L1.module.FS.readFile('/eka2l1/drives/c/data/easyrpg/rgss.trace', { encoding: 'utf8' })`);
            pushLog(`rgss-trace: ${JSON.stringify(result.guestLog)}`);
        } catch (error) {}
        try {
            const ekaLog = await evaluate(`EKA2L1.module.FS.readFile('/EKA2L1.log', { encoding: 'utf8' })`);
            if (typeof ekaLog === 'string' && ekaLog.length)
                fs.writeFileSync(path.join(options.output, 'EKA2L1.log'), ekaLog);
        } catch (error) {}
        if (handoffExitKey && redrawEnd <= redrawStart) throw new Error('Runtime produced no redraws after launcher handoff');
        pushLog(`app: redraw rate ${result.redrawFps.toFixed(2)} fps over ${((sampleEnd - sampleStart) / 1000).toFixed(1)}s`);
        const midiProbe = await evaluate(`(() => {
            const target = '/eka2l1/drives/c/data/easyrpg/audio/bgm.mid';
            try {
                const stat = EKA2L1.module.FS.stat(target);
                return { exists: EKA2L1.module.FS.isFile(stat.mode), size: stat.size };
            } catch (error) {
                return { exists: false, size: 0, error: String(error) };
            }
        })()`);
        pushLog(`midi-temp: ${JSON.stringify(midiProbe)}`);

        if (guestFatal) throw new Error(`Guest runtime fatal: ${guestFatal}`);
        await capture('app-final.png');
        result.success = true;
        result.finishedAt = new Date().toISOString();
        pushLog('result: PASS');
    } catch (error) {
        result.success = false;
        result.finishedAt = new Date().toISOString();
        result.error = error.stack || String(error);
        pushLog(`result: FAIL: ${error.message || error}`);
        if (cachedTrace !== null && !fs.existsSync(path.join(options.output, 'rgss.trace'))) {
            result.guestLog = cachedTrace;
            fs.writeFileSync(path.join(options.output, 'rgss.trace'), cachedTrace);
        }
        if (cdp) {
            try {
                const image = await cdp.call('Page.captureScreenshot', {
                    format: 'png',
                    fromSurface: true,
                    captureBeyondViewport: true
                });
                const file = path.join(options.output, 'failure.png');
                fs.writeFileSync(file, Buffer.from(image.data, 'base64'));
                result.screenshot = file;
            } catch {}
        }
        throw error;
    } finally {
        const finalGuestFatal = guestFatal;
        guestFatal = '';
        if (cdp) {
            try {
                const guestFs = await evaluate(`(() => {
                    const fs = EKA2L1.module.FS;
                    const paths = ['/', '/eka2l1', '/eka2l1/drives', '/eka2l1/drives/c', '/eka2l1/drives/c/data', '/eka2l1/drives/c/Data'];
                    const out = {};
                    for (const target of paths) {
                        try { out[target] = fs.readdir(target); }
                        catch (error) { out[target] = String(error); }
                    }
                    return out;
                })()`);
                result.guestFs = guestFs;
                const guestTrace = await evaluate(`(() => {
                    const fs = EKA2L1.module.FS;
                    const roots = [
                        '/eka2l1/drives/c',
                        '/eka2l1/drives/c/data',
                        '/eka2l1/drives/c/Data'
                    ];
                    const candidates = [];
                    for (const root of roots) {
                        try {
                            for (const name of fs.readdir(root)) {
                                if (name === '.' || name === '..') continue;
                                const level1 = root + '/' + name;
                                candidates.push(level1);
                                try {
                                    for (const child of fs.readdir(level1))
                                        if (child !== '.' && child !== '..') candidates.push(level1 + '/' + child);
                                } catch (error) {}
                            }
                        } catch (error) {}
                    }
                    for (const target of candidates) {
                        if (!target.toLowerCase().endsWith('/rgss.trace')) continue;
                        try { return fs.readFile(target, { encoding: 'utf8' }); }
                        catch (error) {}
                    }
                    return null;
                })()`);
                if (guestTrace) {
                    result.guestLog = guestTrace;
                    fs.writeFileSync(path.join(options.output, 'rgss.trace'), guestTrace);
                }
            } catch {}
            if (!fs.existsSync(path.join(options.output, 'rgss.trace')) && fatalTrace) {
                result.guestLog = fatalTrace;
                fs.writeFileSync(path.join(options.output, 'rgss.trace'), fatalTrace);
            }
        }
        guestFatal = finalGuestFatal;
        fs.writeFileSync(path.join(options.output, 'console.log'), consoleLines.join('\n') + '\n');
        fs.writeFileSync(path.join(options.output, 'result.json'), JSON.stringify(result, null, 2) + '\n');
        fs.writeFileSync(path.join(options.output, 'chrome.log'), chromeStderr);
        if (cdp) cdp.close();
        chrome.kill('SIGTERM');
        await Promise.race([
            new Promise(resolve => chrome.once('exit', resolve)),
            sleep(3000)
        ]);
        if (chrome.exitCode === null) chrome.kill('SIGKILL');
    }
}

main().catch(error => {
    process.stderr.write(`${error.stack || error}\n`);
    process.exitCode = 1;
});
