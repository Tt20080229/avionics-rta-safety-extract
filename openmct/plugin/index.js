// FILE: openmct/plugin/index.js
export default function VerifiedRTAPlugin() {
  return function install(openmct) {
    openmct.types.addType('rta.switch.event', { name: 'RTA Switch Event', creatable: false });
    openmct.telemetry.addProvider({
      supportsRequest: (id) => id.namespace==='rta' && id.key==='verify-stats',
      request: async () => [{ timestamp: Date.now(), value: Math.random()*100 }]
    });
  };
}
