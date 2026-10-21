/* ===================== SHARED SUPABASE CONFIG =====================
   Every page (index.html, patient-dashboard.html, footguard-dashboard.html, admin-dashboard.html)
   loads this ONE file (after the Supabase JS CDN script) instead of hardcoding its own copy — so
   the whole site talks to the same Supabase project, and you only ever need to paste your keys here.

   How to fill this in:
   1. https://supabase.com/dashboard -> create (or open) a project.
   2. Project Settings -> API -> copy the "Project URL" and the "anon public" key.
   3. Paste them below, replacing the "YOUR_..." placeholders.
   4. Table editor -> create the tables listed below (or run the SQL further down).
   5. Authentication -> Providers -> make sure "Email" is enabled.

   TABLES USED ACROSS THE SITE (all in the "public" schema):
     users    (id uuid PK references auth.users, role text, name text, email text, patient_id text, created_at timestamptz)
     patients (id uuid PK references auth.users, patient_id text, personal_details jsonb,
               medical_details jsonb, doctor_details jsonb, created_at timestamptz)
     doctors  (id uuid PK references auth.users, name text, hospital text, specialization text, phone text, created_at timestamptz)
     admins   (id uuid PK references auth.users, name text, organization text, created_at timestamptz)

   LIVE SENSOR TABLE (fed by your ESP32 hardware — see esp32-firmware.ino):
     sensor_data (
       id bigint generated always as identity primary key,
       patient_id text,               -- which patient/device this reading belongs to
       heel numeric,
       midfoot numeric,
       forefoot numeric,
       toe numeric,
       temperature numeric,
       status text,                   -- e.g. 'low' | 'moderate' | 'high' | 'critical'
       created_at timestamptz default now()
     );

   To enable the Realtime subscription used by footguard-dashboard.html:
     Database -> Replication -> turn on Realtime for the "sensor_data" table.

   SQL to create the sensor table quickly (paste into the SQL Editor):
     create table sensor_data (
       id bigint generated always as identity primary key,
       patient_id text,
       heel numeric, midfoot numeric, forefoot numeric, toe numeric,
       temperature numeric, status text,
       created_at timestamptz default now()
     );
     alter publication supabase_realtime add table sensor_data;
*/
window.supabaseConfig = {
  url: "https://rzkqmilgfriogresucxb.supabase.co",
  anonKey: "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6InJ6a3FtaWxnZnJpb2dyZXN1Y3hiIiwicm9sZSI6ImFub24iLCJpYXQiOjE3ODQzNzQ4NjMsImV4cCI6MjA5OTk1MDg2M30.Q0Dbjtyc5c-246w2fnoCHuTLcVxl4ce6uUCu9YAszRc",
  sensorTable: "sensor_data"
};

// Create the shared client once here, so every page just uses window.supabaseClient.
if(window.supabaseConfig.url !== "YOUR_SUPABASE_PROJECT_URL" && window.supabaseConfig.anonKey !== "YOUR_SUPABASE_ANON_KEY"){
  window.supabaseClient = supabase.createClient(window.supabaseConfig.url, window.supabaseConfig.anonKey);
  console.log("Supabase initialized successfully");
} else {
  console.log("Supabase config not set yet — edit supabase-config.js to enable it.");
}
