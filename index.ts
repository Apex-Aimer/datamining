// import parseArgv from "yargs-parser";
import { RON } from "ron-parser";

import { JSONSchema } from "json-schema-to-ts";

// const { f: filePath } = parseArgv(Bun.argv);

const required = [
  "ammo_clip_size",
  "ammo_default_total",
  "damage_near_value",
  "damage_far_value",
  "fire_rate",
] as const;

const optional = [
  "ammo_pool_type",
  "ammo_min_to_fire",
  "fire_mode",
  "projectile_launch_speed",
  "stat_damage",
  "stat_range",
  "stat_accuracy",
  "stat_rof",
  "ammo_stockpile_max",
  "ammo_no_remove_from_stockpile",
  "low_ammo_fraction",
  "damage_very_far_value",
  "deployfirst_time",
  "reload_time",
  "reload_time_late1",
  "reloadempty_time",
  "reloadempty_time_late1",
  "reloadempty_time_late2",
  "viewkick_pattern", // wtf?!
  "viewkick_spring_heatpershot",
  "viewkick_spring_cooldown_holdtime",
  "viewkick_spring_cooldown_fadetime",
  "viewkick_pitch_base",
  "viewkick_pitch_random",
  "viewkick_pitch_softScale",
  "viewkick_pitch_hardScale",
  "viewkick_yaw_base",
  "viewkick_yaw_random",
  "viewkick_yaw_random_innerexclude",
  "viewkick_yaw_softScale",
  "viewkick_yaw_hardScale",
  "viewkick_roll_base",
  "viewkick_roll_randomMin",
  "viewkick_roll_randomMax",
  "viewkick_roll_softScale",
  "viewkick_roll_hardScale",
  "viewkick_hipfire_weaponFraction",
  "viewkick_hipfire_weaponFraction_vmScale",
  "viewkick_ads_weaponFraction",
  "viewkick_ads_weaponFraction",
  "viewkick_ads_weaponFraction_vmScale",
  "viewkick_scale_firstshot_hipfire",
  "viewkick_scale_min_hipfire",
  "viewkick_scale_max_hipfire",
  "viewkick_scale_firstshot_ads",
  "viewkick_scale_min_ads",
  "viewkick_scale_max_ads",
  "viewkick_scale_valuePerShot",
  "viewkick_scale_pitch_valueLerpStart",
  "viewkick_scale_pitch_valueLerpEnd",
  "viewkick_scale_yaw_valueLerpStart",
  "viewkick_scale_yaw_valueLerpEnd",
  "viewkick_scale_valueDecayDelay",
  "viewkick_scale_valueDecayRate",
  "viewkick_perm_pitch_base",
  "viewkick_perm_pitch_random",
  "viewkick_perm_yaw_base",
  "viewkick_perm_yaw_random",
  "viewkick_perm_yaw_random_innerexclude",
  "viewmodel_shake_forward",
  "sprintcycle_time",
  "clip_bodygroup",
] as const;

const weaponObject = {
  type: "object",
  required,
  properties: {
    ammo_pool_type: { type: "string" },
    fire_mode: { type: "string" },
    projectile_launch_speed: {
      type: "string",
    },
    stat_damage: { type: "string" },
    stat_range: { type: "string" },
    stat_accuracy: { type: "string" },
    stat_rof: { type: "string" },
    ammo_clip_size: { type: "string" },
    ammo_min_to_fire: { type: "string" },
    ammo_default_total: { type: "string" },
    ammo_stockpile_max: { type: "string" },
    ammo_no_remove_from_stockpile: { type: "string" },
    low_ammo_fraction: { type: "string" },
    damage_near_value: { type: "string" },
    damage_far_value: { type: "string" },
    damage_very_far_value: { type: "string" },
    deployfirst_time: { type: "string" },
    fire_rate: { type: "string" },
    reload_time: { type: "string" },
    reload_time_late1: { type: "string" },
    reloadempty_time: { type: "string" },
    reloadempty_time_late1: { type: "string" },
    reloadempty_time_late2: { type: "string" },
    viewkick_pattern: { type: "string" },
    viewkick_spring_heatpershot: { type: "string" },
    viewkick_spring_cooldown_holdtime: { type: "string" },
    viewkick_spring_cooldown_fadetime: { type: "string" },
    viewkick_pitch_base: { type: "string" },
    viewkick_pitch_random: { type: "string" },
    viewkick_pitch_softScale: { type: "string" },
    viewkick_pitch_hardScale: { type: "string" },
    viewkick_yaw_base: { type: "string" },
    viewkick_yaw_random: { type: "string" },
    viewkick_yaw_random_innerexclude: { type: "string" },
    viewkick_yaw_softScale: { type: "string" },
    viewkick_yaw_hardScale: { type: "string" },
    viewkick_roll_base: { type: "string" },
    viewkick_roll_randomMin: { type: "string" },
    viewkick_roll_randomMax: { type: "string" },
    viewkick_roll_softScale: { type: "string" },
    viewkick_roll_hardScale: { type: "string" },
    viewkick_hipfire_weaponFraction: { type: "string" },
    viewkick_hipfire_weaponFraction_vmScale: {
      type: "string",
    },
    viewkick_ads_weaponFraction: { type: "string" },
    viewkick_ads_weaponFraction_vmScale: { type: "string" },
    viewkick_scale_firstshot_hipfire: { type: "string" },
    viewkick_scale_min_hipfire: { type: "string" },
    viewkick_scale_max_hipfire: { type: "string" },
    viewkick_scale_firstshot_ads: { type: "string" },
    viewkick_scale_min_ads: { type: "string" },
    viewkick_scale_max_ads: { type: "string" },
    viewkick_scale_valuePerShot: { type: "string" },
    viewkick_scale_pitch_valueLerpStart: { type: "string" },
    viewkick_scale_pitch_valueLerpEnd: { type: "string" },
    viewkick_scale_yaw_valueLerpStart: { type: "string" },
    viewkick_scale_yaw_valueLerpEnd: { type: "string" },
    viewkick_scale_valueDecayDelay: { type: "string" },
    viewkick_scale_valueDecayRate: { type: "string" },
    viewkick_perm_pitch_base: { type: "string" },
    viewkick_perm_pitch_random: { type: "string" },
    viewkick_perm_yaw_base: { type: "string" },
    viewkick_perm_yaw_random: { type: "string" },
    viewkick_perm_yaw_random_innerexclude: { type: "string" },
    viewmodel_shake_forward: { type: "string" },
    sprintcycle_time: { type: "string" },
    clip_bodygroup: { type: "string" },
    // TODO
    Mods: {
      type: "object",
      properties: {
        altfire: {
          type: "object",
          properties: {
            mod_activity_modifier: { type: "string" },
            is_semi_auto: { type: "string" },
          },
        },
        gold_paintball: {
          type: "object",
          properties: {
            impact_effect_table: { type: "string" },
            reload_time: { type: "string" },
            reload_time_late1: { type: "string" },
            reload_time_late2: { type: "string" },
            reload_time_late3: { type: "string" },
            reload_time_late4: { type: "string" },
            reload_time_late5: { type: "string" },
            reloadempty_time: { type: "string" },
            reloadempty_time_late1: { type: "string" },
            reloadempty_time_late2: { type: "string" },
            reloadempty_time_late3: { type: "string" },
            reloadempty_time_late4: { type: "string" },
            reloadempty_time_late5: { type: "string" },
          },
        },
        altfire_highcal: {
          type: "object",
          properties: {
            mod_activity_modifier: { type: "string" },
            is_semi_auto: { type: "string" },
          },
        },
        bullets_mag_l1: {
          type: "object",
          properties: {
            ammo_clip_size: { type: "string" },
          },
        },
        bullets_mag_l2: {
          type: "object",
          properties: {
            ammo_clip_size: { type: "string" },
          },
        },
        bullets_mag_l3: {
          type: "object",
          properties: {
            ammo_clip_size: { type: "string" },
          },
        },
        bullets_mag_l4: {
          type: "object",
          properties: {
            ammo_clip_size: { type: "string" },
          },
        },
      },
    },
  },
} as const satisfies JSONSchema;

async function parseWeapon(filePath: string) {
  const file = await Bun.file(filePath).text();
  const data = await new RON().parse(file, {
    type: "object",
    properties: {
      WeaponData: weaponObject,
    },
    required: ["WeaponData"],
  } as const satisfies JSONSchema);

  return data;
}

(async function main() {
  const weapons = {
    flatline: await parseWeapon(
      "./output/common.rpak/r2\\scripts\\weapons\\mp_weapon_rspn101.txt"
    ),
    /**
     * 454 line - "highca l_mag_l1" -> "highcal_mag_l1"
     */
    3030: await parseWeapon(
      "./output/common.rpak/r2\\scripts\\weapons\\mp_weapon_3030.txt"
    ),
    r301: await parseWeapon(
      "./output/common.rpak/r2\\scripts\\weapons\\mp_weapon_rspn101.txt"
    ),
    alternator: await parseWeapon(
      "./output/common.rpak/r2\\scripts\\weapons\\mp_weapon_alternator_smg.txt"
    ),
    car: await parseWeapon(
      "./output/common.rpak/r2\\scripts\\weapons\\mp_weapon_car.txt"
    ),
    g2: await parseWeapon(
      "./output/common.rpak/r2\\scripts\\weapons\\mp_weapon_g2.txt"
    ),
    hemlok: await parseWeapon(
      "./output/common.rpak/r2\\scripts\\weapons\\mp_weapon_hemlok.txt"
    ),
    lstar: await parseWeapon(
      "./output/common.rpak/r2\\scripts\\weapons\\mp_weapon_lstar.txt"
    ),
    mastiff: await parseWeapon(
      "./output/common.rpak/r2\\scripts\\weapons\\mp_weapon_mastiff.txt"
    ),
    nemesis: await parseWeapon(
      "./output/common.rpak/r2\\scripts\\weapons\\mp_weapon_nemesis.txt"
    ),
    r99: await parseWeapon(
      "./output/common.rpak/r2\\scripts\\weapons\\mp_weapon_r97.txt"
    ),
    sentinel: await parseWeapon(
      "./output/common.rpak/r2\\scripts\\weapons\\mp_weapon_sentinel.txt"
    ),
    throwingKnife: await parseWeapon(
      "./output/common.rpak/r2\\scripts\\weapons\\mp_weapon_throwingknife.txt"
    ),
    voltSmg: await parseWeapon(
      "./output/common.rpak/r2\\scripts\\weapons\\mp_weapon_volt_smg.txt"
    ),
    wingman: await parseWeapon(
      "./output/common.rpak/r2\\scripts\\weapons\\mp_weapon_wingman.txt"
    ),
    bow: await parseWeapon(
      "./output/common.rpak/r2\\scripts\\weapons\\mp_weapon_bow.txt"
    ),
    chargeRifle: await parseWeapon(
      "./output/common.rpak/r2\\scripts\\weapons\\mp_weapon_defender.txt"
    ),
  } as const;

  type WeaponsKeys = keyof typeof weapons;

  const properties = [...required, ...optional];

  const csv = `
Properties,${Object.keys(weapons).join(",")}
${properties
  .map((property) => {
    return `${property},${Object.keys(weapons)
      .map(
        (weapon) =>
          weapons[weapon as WeaponsKeys].WeaponData?.[property] || "None"
      )
      .join(",")}`;
  })
  .join("\n")}`;

  await Bun.write("output/weapons.csv", csv);
})();
